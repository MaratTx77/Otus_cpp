
#include <iostream>
#include <cstdint>
#include <atomic>
#include <vector>
#include <map>


/* Идея allocator-а:
 * 1) Изначально есть кусочки памяти разной длины, которые можем выдавать по требованию, если их не хватает - запрашиваем у ОС
 * 2) Изначальные кусочки delta_size*sizeof(T), 2*delta_size*sizeof(T), 3*delta_size*sizeof(T)... 10*delta_size*sizeof(T)
 * Для мапы у которого значения фиксированной длины - это плохо, но я дальше буду делать для мапы бинарников
 * Один и тот же аллокатор может обслуживать разные коллекции одного типа
*/

constexpr static size_t ALLOC_SECRET = 2023110611;

template <class T, size_t delta_size = 10, size_t start_buffer_count = 10, size_t max_buffer_count = 100>
class BufferAllocator {
    struct Buffer { // Буферы разной длины нашего аллокатора
        char* data; // указатель на буфер (с учетом его заголовка)
        size_t size; // размер буфера в элементах T
        bool is_used = false;
    };

    struct BufferHeader { // Префиксный заголовок нашего буфера
        size_t index; // номер буфера
        size_t size; // размер буфера в байтах
    };

    Buffer buffer[max_buffer_count];
    size_t buffer_count = start_buffer_count;

public:
    using value_type = T;
    BufferAllocator() noexcept {
        #if defined(NDEBUG) // Для отладки
            std::cout << "constructor " << delta_size << std::endl;
        #endif

        void  *buf_array[start_buffer_count];
        for (size_t i = 0; i < start_buffer_count; ++i) {

            size_t data_size = (i+1) * delta_size * sizeof(T);
            buf_array[i] = malloc (data_size + sizeof (BufferHeader));

            if (buf_array[i]) {
                buffer[i].data = static_cast<char *>(buf_array[i]);
                buffer[i].is_used = false;
                buffer[i].size = (i+1) * delta_size;

                auto *pBufferHeader  =  reinterpret_cast<BufferHeader *>(buffer[i].data);
                pBufferHeader->size = data_size + sizeof (BufferHeader); // Оформляем заголовок
                pBufferHeader->index = i;
                #if defined(NDEBUG)
                    std::cout << "create buffer " << i <<  " size=" << data_size << " bytes" << std::endl;
                #endif
            } else {
                buffer[i].size = 0; // память не была выделена
                buffer[i].is_used = false;
            }
        }
        std::cout << std::endl;
    }

    ~BufferAllocator() {
        for(size_t i = 0; i < buffer_count; ++i)
                free(buffer[i].data);
    }

    template <class U>
    BufferAllocator  (const BufferAllocator<U, delta_size, start_buffer_count, max_buffer_count>&) noexcept {}

    T* allocate (std::size_t n) {
        size_t buffer_size = (n/delta_size)*delta_size + delta_size;

        #if defined(NDEBUG)
            std::cout << "try allocate size for " << n << " elements" << std::endl;
        #endif

        // Проверяем свободны ли текущие буферы и если да - выделяем один из них
        for(size_t i = 0; i < buffer_count; ++i)
            if (!buffer[i].is_used && buffer[i].size >= n) { // Есть пустой подходящий по размеру буфер
                buffer[i].is_used = true;

                #if defined(NDEBUG)
                    std::cout << "use buffer =" << i << std::endl;
                #endif

                return reinterpret_cast<T*>(buffer[i].data + sizeof (BufferHeader));
            }

        size_t buffer_idx = 0;

        // если предельное кол-во буферов достигнуто - создадим новый
        if (buffer_count == max_buffer_count) { // если число буферов предельно, освободим неиспользуемый старый
            for (size_t i = 0; i < buffer_count; ++i)
                if (!buffer[i].is_used) {
                    free(buffer[i].data);
                    buffer_idx = i;

                    #if defined(NDEBUG)
                        std::cout << "use existed buffer =" << i << std::endl;
                    #endif
                }
        } else { // если предел не достигнут - создадим новый буфер
            buffer_idx = buffer_count++; // если предел не достигнут - сделаем новый

            #if defined(NDEBUG)
                std::cout << "create new buffer=" << buffer_idx << std::endl;
            #endif
        }

        if (buffer_idx) { // собственно создание буфера
            std::cout << "create buffer" << n << std::endl;

            void *buf_array = malloc(buffer_size * sizeof(T) + sizeof (BufferHeader));

            if (buf_array) {
                buffer[buffer_idx].data = static_cast<char *> (buf_array);
                buffer[buffer_idx].size = buffer_size;
                buffer[buffer_idx].is_used = true;

                auto *pBufferHeader = reinterpret_cast<BufferHeader *>(buffer[buffer_idx].data);
                pBufferHeader->size = buffer_size; // Оформляем заголовок
                pBufferHeader->index = buffer_idx;

                std::cout << "create buffer" << buffer_idx << " size=" <<  buffer_size << std::endl;
                std::cout << std::endl;

                // Возвращаем адрес размещения данных (смещая на размер буфера)
                return reinterpret_cast<T*>(buffer[buffer_idx].data + sizeof (BufferHeader));
            }
        }
        #if defined(NDEBUG)
            std::cout << "maximum allowed buffer are riched" << std::endl;
        #endif

        throw std::bad_alloc(); // превысили заданные параметрами шаблона пределы
    }

    void deallocate(T* ptr, [[maybe_unused]] std::size_t n) noexcept {
        #if defined(NDEBUG)
            std::cout << "deallocate size=" << n << std::endl;
        #endif

        char *block = reinterpret_cast<char *>(ptr) - sizeof(BufferHeader);
        auto *pBufferHeader  =  reinterpret_cast<BufferHeader *>(block);
        buffer[pBufferHeader->index].is_used = false; // Помечаем буфер как неиспользуемый
    }

    template<class U>
    struct rebind {
        typedef BufferAllocator<U, delta_size, start_buffer_count, max_buffer_count > other;
    };
};

template <class T, class U>
constexpr bool operator== (const BufferAllocator<T>& a1, const BufferAllocator<U>& a2) noexcept {
    return true;
}

template <class T, class U>
constexpr bool operator!= (const BufferAllocator<T>& a1, const BufferAllocator<U>& a2) noexcept{
    return false;
}

// Согласно задания map-а с фиксированным сверху максимальным размером элементов
template< class Key,
          class T,
          size_t MAX,
          class Compare = std::less<Key>,
          class Allocator = BufferAllocator<std::pair<const Key, T>, 10, 10, MAX>>
class Map_Fixed_Size : public std::map<Key, T, Compare, Allocator> {};

// Согласно задания int-контейнер с фиксированным сверху максимальным размером элементов
template<size_t MAX,
         class Allocator = BufferAllocator<int, 2, MAX/2, MAX>>
class Int_Cont : public std::vector<int, Allocator> {};

int main (int, char **) {
    // создание экземпляра std::map<int, int> + заполнение факториалом
    std::map< int, int> m1;
    m1[0] = 1;
    for (int i = 1; i < 10; ++i)
        m1[i] = m1[i-1]*i;

    // создание экземпляра std::map<int, int> с новым аллокатором + заполнение факториалом
    Map_Fixed_Size<int, int, 10> m2;
    m2[0] = 1;
    for (int i = 1; i < 10; ++i)
        m2[i] = m2[i-1]*i;

    for (int i = 0; i < 10; ++i)
        std::cout << "std::map[" << i << "] " << m1[i] << " Map_Fixed_Size[" << i << "] " << m2[i] << std::endl;

    //создание экземпляра своего контейнера для хранения значений типа int с новым аллокатором, ограниченным 10 элементами
    Int_Cont<10> cont;
    cont.push_back(1);
    int a = 1;
    for (int i = 1; i < 10; ++i) { // заполнение факториалом
        a *= i;
        cont.push_back(a);
    }

    // вывод на экран всех значений, хранящихся в контейнере
    for (int i = 0; i < 10; ++i)
        std::cout << "Int_Cont " << i << " " << cont[i] << std::endl;

    return 0;
}
