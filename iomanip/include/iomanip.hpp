/* Custom manipulators.
 * @file iomanip
 * @date 2020
 * @author Rysina_S
 */

#include <iostream>

#ifndef __IOMANIP_HPP__
#define __IOMANIP_HPP__

//{ endm manipulator

inline std::ostream& endm(std::ostream& out) {
    return out << "[eol]\n";
}

//}


//{ squares manipulator

class SquaresContainer {
    private:
        std::ostream* m_os;

    public:
        SquaresContainer(std::ostream* os)
            : m_os(os)
        {}

        template<
            typename T,
            typename = decltype(
                std::declval<std::ostream>() << std::declval<T>()
            )
        >
        decltype(auto) operator<<(const T& str) {
            return ((*m_os) << '[' << str << ']');
        }
};

struct Square {
    friend decltype(auto) operator<<(std::ostream& os, Square) {
        return SquaresContainer(&os);
    }
};

const Square squares;
//}


//{ add manipulator

template<
    typename T,
    typename = decltype(
        std::declval<T>() + std::declval<T>()
    )
>
class AddContainerSecond {
    private:
        std::ostream* m_os;
        T             m_s1;

    public:
        AddContainerSecond(std::ostream* os, const T& s1)
            : m_os(os)
            , m_s1(s1)
        {}

        decltype(auto) operator<<(const T& s2) {
            return (*m_os) << m_s1 + s2;
        }
};

class AddContainerFirst {
    private:
        std::ostream* m_os;

    public:
        AddContainerFirst(std::ostream* os)
            : m_os(os)
        {}

        template<typename T>
        friend decltype(auto) operator<<(AddContainerFirst f1, const T& s1) {
            return AddContainerSecond(f1.m_os, s1);
        }
};

struct Add {
    friend decltype(auto) operator<<(std::ostream& os, Add) {
        return AddContainerFirst(&os);
    }
};

const Add add;

//}

#endif // __IOMANIP_HPP__
