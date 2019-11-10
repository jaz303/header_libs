#ifndef RING_BUFFER_HPP
#define RING_BUFFER_HPP

// Size must be a power of two!
template<class T>
class ring_buffer {

public:
    ring_buffer(T *items, int size) : items_(items), size_(size), mask_(size - 1), rp_(0), wp_(0), count_(0), owned_(false) {}

#ifndef JLIB_NO_MALLOC
    ring_buffer(int size) : items_(new T[size]), size_(size), mask_(size - 1), rp_(0), wp_(0), count_(0), owned_(true) {}
    ring_buffer(T *items, int size, bool adopt) : items_(items), size_(size), mask_(size - 1), rp_(0), wp_(0), count_(0), owned_(adopt) {}
#endif

    ~ring_buffer() {
#ifndef JLIB_NO_MALLOC
        if (owned_) {
            delete[] items_;    
        }
#endif
    }

    bool is_full() { return count_ == size_; }
    bool is_empty() { return count_ == 0; }
    int count() { return count_; }

    bool add(T item) {
        if (is_full()) {
            return false;
        }
        items_[wp_] = item;
        wp_ = (wp_ + 1) & mask_;
        count_++;
        return true;
    }

    bool remove(T *ptr) {
        if (is_empty()) {
            return false;
        }
        *ptr = do_remove();
        return true;
    }

    T remove() {
        if (is_empty()) {
            return T();
        }
        return do_remove();
    }

    T* remove_ptr() {
        if (is_empty()) {
            return nullptr;
        }
        int rx = rp_;
        rp_ = (rp_ + 1) & mask_;
        count_--;
        return &items_[rx];
    }

private:
    T do_remove() {
        int rx = rp_;
        rp_ = (rp_ + 1) & mask_;
        count_--;
        return items_[rx];
    }

    T *items_;
    int size_, mask_;
    int rp_, wp_, count_;
    bool owned_;
};

#endif
