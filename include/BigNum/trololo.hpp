#include <cstddef>
#include <type_traits>

struct NullType
{
};

template <typename... T> struct TypeList
{
    using Head = NullType;
    using Tail = NullType;
};

template <typename H, typename... T> struct TypeList<H, T...>
{
    using Head = H;
    using Tail = TypeList<T...>;
};

using EmptyTypeList = TypeList<>;

template <typename TL> struct IsEmpty : std::false_type
{
};

template <> struct IsEmpty<EmptyTypeList> : std::true_type
{
};

template <typename TL> struct Length : std::integral_constant<size_t, 0>
{
};

template <typename... Args>
struct Length<TypeList<Args...>>
    : std::integral_constant<
          size_t, IsEmpty<TypeList<Args...>>::value
                      ? 0
                      : 1 + Length<typename TypeList<Args...>::Tail>::value>
{
};

template <size_t i, typename TL> struct TypeAt
{
    using type = NullType;
};

template <typename... Args> struct TypeAt<0, TypeList<Args...>>
{
    using type = typename TypeList<Args...>::Head;
};

template <size_t i, typename... Args> struct TypeAt<i, TypeList<Args...>>
{
    static_assert(i < Length<TypeList<Args...>>::value, "Index out of bounds");
    using type = typename TypeAt<i - 1, typename TypeList<Args...>::Tail>::type;
};

void lol();
