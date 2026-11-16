#include <cstddef>
#include <utility>
#include <stdexcept>

class Mtx {};

class Mtxs {
public:
    Mtxs();
    Mtxs(size_t count);
    Mtxs(const Mtxs& oth);
    Mtxs(Mtxs&& oth) noexcept;
    Mtxs& operator=(const Mtxs& oth);
    Mtxs& operator=(Mtxs&& oth) noexcept;
    ~Mtxs();

    size_t getSize() const noexcept;
    Mtx& operator[](size_t id) noexcept;
    const Mtx& operator[](size_t id) const noexcept;

    Mtxs erase(size_t from, size_t to) const;
    Mtxs erase(size_t id) const;

    Mtxs insert(size_t id, const Mtxs& src) const;
    Mtxs insert(size_t id, const Mtx& item) const;

    Mtxs operator+(const Mtx& item) const;
    Mtxs operator+(const Mtxs& oth) const;
    Mtxs& operator+=(const Mtx& item);
    Mtxs& operator+=(const Mtxs& oth);

private:
    Mtx* data_;
    size_t size_;
};


Mtxs::Mtxs():
    data_(nullptr),
    size_(0)
{}

Mtxs::Mtxs(size_t count):
    data_(new Mtx[count]),
    size_(count)
{}

Mtxs::Mtxs(const Mtxs& oth):
    data_(new Mtx[oth.size_]),
    size_(oth.size_)
{
    try {
        for (size_t i = 0; i < size_; ++i) {
            data_[i] = oth.data_[i];
        }
    }
    catch (...) {
        delete[] data_;
        throw;
    }
}

Mtxs::Mtxs(Mtxs&& oth) noexcept:
    data_(oth.data_),
    size_(oth.size_)
{
    oth.data_ = nullptr;
    oth.size_ = 0;
}

Mtxs& Mtxs::operator=(const Mtxs& oth) {
    if (this == &oth) {
        return *this;
    }
    Mtx* buf = new Mtx[oth.size_];
    try {
        for (size_t i = 0; i < oth.size_; ++i) {
            buf[i] = oth.data_[i];
        }
    }
    catch (...) {
        delete[] buf;
        throw;
    }
    delete[] data_;
    data_ = buf;
    size_ = oth.size_;
    return *this;
}

Mtxs& Mtxs::operator=(Mtxs&& oth) noexcept {
    if (this == &oth) {
        return *this;
    }
    delete[] data_;
    data_ = oth.data_;
    size_ = oth.size_;
    oth.data_ = nullptr;
    oth.size_ = 0;
    return *this;
}

Mtxs::~Mtxs() {
    delete[] data_;
}

size_t Mtxs::getSize() const noexcept {
    return size_;
}

Mtx& Mtxs::operator[](size_t id) noexcept {
    return data_[id];
}

const Mtx& Mtxs::operator[](size_t id) const noexcept {
    return data_[id];
}

Mtxs Mtxs::erase(size_t from, size_t to) const {
    if (to > size_ || from > to) {
        throw std::logic_error("invalid range");
    }

    Mtxs res(size_ - (to - from));
    size_t j = 0;

    for (size_t i = 0; i < from; ++i) {
        res.data_[j++] = data_[i];
    }
    for (size_t i = to; i < size_; ++i) {
        res.data_[j++] = data_[i];
    }

    return res;
}

Mtxs Mtxs::erase(size_t id) const {
    if (id >= size_) {
        throw std::logic_error("index out of range");
    }
    return erase(id, id + 1);
}

Mtxs Mtxs::insert(size_t id, const Mtxs& src) const {
    if (id > size_) {
        throw std::logic_error("index out of range");
    }

    Mtxs res(size_ + src.size_);
    size_t j = 0;

    for (size_t i = 0; i < id; ++i) {
        res.data_[j++] = data_[i];
    }
    for (size_t i = 0; i < src.size_; ++i) {
        res.data_[j++] = src.data_[i];
    }
    for (size_t i = id; i < size_; ++i) {
        res.data_[j++] = data_[i];
    }

    return res;
}

Mtxs Mtxs::insert(size_t id, const Mtx& item) const {
    if (id > size_) {
        throw std::logic_error("index out of range");
    }

    Mtxs res(size_ + 1);
    size_t j = 0;

    for (size_t i = 0; i < id; ++i) {
        res.data_[j++] = data_[i];
    }
    res.data_[j++] = item;
    for (size_t i = id; i < size_; ++i) {
        res.data_[j++] = data_[i];
    }

    return res;
}

Mtxs Mtxs::operator+(const Mtx& item) const {
    return insert(size_, item);
}

Mtxs Mtxs::operator+(const Mtxs& oth) const {
    return insert(size_, oth);
}

Mtxs& Mtxs::operator+=(const Mtx& item) {
    *this = *this + item;
    return *this;
}

Mtxs& Mtxs::operator+=(const Mtxs& oth) {
    *this = *this + oth;
    return *this;
}