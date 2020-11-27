/* Image iterator.
 * @file
 * @date 2020
 * @author Серафима Рысина
 */

#ifndef __ITERATOR_HPP__
#define __ITERATOR_HPP__

#include <algorithm>
#include <type_traits>
#include <vector>
#include <utility>

#include <boost/iterator/iterator_adaptor.hpp>

#include <iostream>

template<typename T>
class image_iterator : public boost::iterator_adaptor<image_iterator<T>, T>
{
    public:
        image_iterator(T it, size_t w, size_t st)
            : boost::iterator_adaptor<image_iterator<T>, T>(it)
            , start_iterator_(it)
            , w_(w)
            , st_(st)
        {}

        long distance_to(const image_iterator& it) const
        {
            const auto d1 = std::abs(it.base() - it.start_iterator_) % ptrdiff_t(st_);
            const auto d2 = std::abs(this->base() - start_iterator_) % ptrdiff_t(st_);

            return (it.base() - d1 - (this->base() - d2)) / ptrdiff_t(st_) * w_  + (d1 - d2);
        }

        void advance(const int n)
        {
            if (n > 0) for (int i = 0; i < n; ++i) increment();            
            else for (int i = 0; i < -n; ++i) decrement();
        }

        void decrement()
        {
            if (std::abs(this->base() - start_iterator_) % ptrdiff_t(st_) <= 0)
                this->base_reference() -= (st_ - w_ + 1);
            else
                --this->base_reference();
        }

        void increment() {
            if (std::abs(this->base() - start_iterator_) % ptrdiff_t(st_) + 1 >= long(w_))
                this->base_reference() += (st_ - w_ + 1);
            else
                ++this->base_reference();
        }                

    private:
        T start_iterator_;
        size_t w_;
        size_t st_;
};

template<class Container = std::vector<uint8_t>>
class image
{
public:
    image(size_t width, size_t height, size_t stride):
        data(stride * height),
        width_(width),
        stride_(stride)
    {}

    auto begin()
    {
        return image_iterator(data.begin(), width_, stride_);
    }

    auto end()
    {
        return image_iterator(data.end(), width_, stride_);
    }

    auto begin() const
    {
        return image_iterator(data.begin(), width_, stride_);
    }

    auto end() const
    {
        return image_iterator(data.end(), width_, stride_);
    }

private:
    Container data; // image data including strides. Access by (x, y): data[y * stride_ + x]
    size_t width_;
    size_t stride_;
};

#endif // __ITERATOR_HPP__
