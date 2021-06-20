
#ifndef REACTIVE_CLASS_HEADER_FILE
#define REACTIVE_CLASS_HEADER_FILE
#include <functional>
#include <memory>

#define REACTIVE_GENERATE_UNARY_OPERATOR(_operator) \
Reactive operator _operator() \
{ \
    return makeUnaryPred(*this, [](const T& op) { return static_cast<T>(_operator op); }); \
}

#define REACTIVE_GENERATE_BINARY_OPERATOR(_operator) \
    Reactive operator _operator(const Reactive& other) \
    { \
        return makeBinaryPred(*this, other, [](const T& lhs, const T& rhs) { return static_cast<T>(lhs _operator rhs); }); \
    } \
    Reactive operator _operator(const T& other) \
    {return makeBinaryPred(*this, other, [](const T& lhs, const T& rhs) { return static_cast<T>(lhs _operator rhs); });} \


// to support the  T being on the left hand side like   v1 = 1 + v2;
#define REACTIVE_GENERATE_STATIC_BINARY_OPERATOR(_operator) \
    template<typename T> \
    static Reactive<T> operator _operator(const T& lhs,const Reactive<T>& rhs) \
    {  return Reactive<T>(lhs) _operator rhs;  }

template<typename T>
class Reactive
{
    using FnType = std::function<T()>;
    std::shared_ptr<FnType> evaluateFunc;

public:

    Reactive() : evaluateFunc(std::make_shared<FnType>([]() -> T { return {}; }))
    {
    }

    Reactive(const T& init)
        : evaluateFunc(std::make_shared<FnType>([init]() {return init; }))
    {
    }

    Reactive(T&& init)
        : evaluateFunc(std::make_shared<FnType>(
            [init_m = std::move(init)]() {return init_m; }))
    {
    }
    Reactive(const Reactive& other)
        : evaluateFunc(std::make_shared<FnType>())
    {
        this->operator =(other);
    }
    Reactive(Reactive&& other) noexcept
        : evaluateFunc(std::make_shared<FnType>())
    {
        this->operator =(std::move(other));
    }

    Reactive& operator =(const Reactive& other)
    {
        *evaluateFunc = [other_ptr = other.evaluateFunc]() {return (*other_ptr)(); };
        return *this;
    }
    Reactive& operator =(Reactive&& other) noexcept
    {
        *evaluateFunc = [other_ptr = std::move(other.evaluateFunc)]() {return (*other_ptr)(); };
        return *this;
    }
    Reactive& operator =(const T& other)
    {
        *evaluateFunc = [other]() {return other; };
        return *this;
    }
    Reactive& operator =(T&& other) noexcept
    {
        *evaluateFunc = [other_m = std::move(other)]() {return other_m; };
        return *this;
    }

    operator T() const
    {
        return (*evaluateFunc)();
    }



    static Reactive makeBinaryPred(const Reactive& lhs, const Reactive& rhs, T(*Pred)(const T&, const T&))
    {
        Reactive result;
        *result.evaluateFunc = [Pred, lhs_ptr = lhs.evaluateFunc,
            rhs_ptr = rhs.evaluateFunc]()->T
        {
            return Pred((*lhs_ptr)(), (*rhs_ptr)());
        };
        return result;
    }

    static Reactive makeUnaryPred(const Reactive& op, T(*Pred)(const T&))
    {
        Reactive result;
        *result.evaluateFunc = [Pred, op_ptr = op.evaluateFunc]()->T
        {
            return Pred((*op_ptr)());
        };
        return result;
    }



    // arithmetic operators
    REACTIVE_GENERATE_BINARY_OPERATOR(+);
    REACTIVE_GENERATE_BINARY_OPERATOR(-);
    REACTIVE_GENERATE_BINARY_OPERATOR(*);
    REACTIVE_GENERATE_BINARY_OPERATOR(/ );
    REACTIVE_GENERATE_BINARY_OPERATOR(%);

    // logical operators
    REACTIVE_GENERATE_UNARY_OPERATOR(!);
    REACTIVE_GENERATE_BINARY_OPERATOR(&&);
    REACTIVE_GENERATE_BINARY_OPERATOR(|| );

    // bitwise operators
    REACTIVE_GENERATE_UNARY_OPERATOR(~);
    REACTIVE_GENERATE_BINARY_OPERATOR(| );
    REACTIVE_GENERATE_BINARY_OPERATOR(&);
    REACTIVE_GENERATE_BINARY_OPERATOR(^);
    REACTIVE_GENERATE_BINARY_OPERATOR(>> );
    REACTIVE_GENERATE_BINARY_OPERATOR(<< );


    // copmarison operators
    REACTIVE_GENERATE_BINARY_OPERATOR(> );
    REACTIVE_GENERATE_BINARY_OPERATOR(< );
    REACTIVE_GENERATE_BINARY_OPERATOR(== );
    REACTIVE_GENERATE_BINARY_OPERATOR(!= );
    REACTIVE_GENERATE_BINARY_OPERATOR(>= );
    REACTIVE_GENERATE_BINARY_OPERATOR(<= );
};


// arithmetic operators
REACTIVE_GENERATE_STATIC_BINARY_OPERATOR(+);
REACTIVE_GENERATE_STATIC_BINARY_OPERATOR(-);
REACTIVE_GENERATE_STATIC_BINARY_OPERATOR(*);
REACTIVE_GENERATE_STATIC_BINARY_OPERATOR(/ );
REACTIVE_GENERATE_STATIC_BINARY_OPERATOR(%);

// logical operators
REACTIVE_GENERATE_STATIC_BINARY_OPERATOR(&&);
REACTIVE_GENERATE_STATIC_BINARY_OPERATOR(|| );

// bitwise operators
REACTIVE_GENERATE_STATIC_BINARY_OPERATOR(| );
REACTIVE_GENERATE_STATIC_BINARY_OPERATOR(&);
REACTIVE_GENERATE_STATIC_BINARY_OPERATOR(^);
REACTIVE_GENERATE_STATIC_BINARY_OPERATOR(>> );
REACTIVE_GENERATE_STATIC_BINARY_OPERATOR(<< );


// copmarison operators
REACTIVE_GENERATE_STATIC_BINARY_OPERATOR(> );
REACTIVE_GENERATE_STATIC_BINARY_OPERATOR(< );
REACTIVE_GENERATE_STATIC_BINARY_OPERATOR(== );
REACTIVE_GENERATE_STATIC_BINARY_OPERATOR(!= );
REACTIVE_GENERATE_STATIC_BINARY_OPERATOR(>= );
REACTIVE_GENERATE_STATIC_BINARY_OPERATOR(<= );

#endif