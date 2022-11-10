#pragma once

#include <type_traits>

namespace CPPS {

template<typename... T>
struct TypeList
{
};

template<typename TargetTypeT, typename... TypesT>
struct TypesContains
{
    static constexpr bool Value{(std::is_same_v<TargetTypeT, TypesT> || ...)};
};

template<typename TargetTypeT, typename TypeListT>
struct TypeListContains;

template<typename TargetTypeT, typename... TypesT>
struct TypeListContains<TargetTypeT, TypeList<TypesT...>> : TypesContains<TargetTypeT, TypesT...>
{};

template<typename TargetTypeT, typename TypeListT>
constexpr bool TypeListContainsV{TypeListContains<TargetTypeT, TypeListT>::Value};

template<typename TargetTypeT, size_t IndexT, typename... TypesT>
struct IndexOfFromTypesImpl;

template<typename TargetTypeT, size_t IndexT, typename HeadT>
struct IndexOfFromTypesImpl<TargetTypeT, IndexT, HeadT>
{
};

template<typename TargetTypeT, size_t IndexT>
struct IndexOfFromTypesImpl<TargetTypeT, IndexT, TargetTypeT>
{
    static constexpr size_t Value{IndexT};
};

template<typename TargetTypeT, size_t IndexT, typename HeadT, typename... TailsT>
struct IndexOfFromTypesImpl<TargetTypeT, IndexT, HeadT, TailsT...>
    : IndexOfFromTypesImpl<TargetTypeT, IndexT, HeadT>, IndexOfFromTypesImpl<TargetTypeT, IndexT + 1, TailsT...>
{
};

template<typename TargetTypeT, typename... TypesT>
struct IndexOfFromTypes : IndexOfFromTypesImpl<TargetTypeT, 0, TypesT...>
{
};

template<typename TargetTypeT, typename TypeListT>
struct IndexOfFromTypeList;

template<typename TargetTypeT, typename... TypesT>
struct IndexOfFromTypeList<TargetTypeT, TypeList<TypesT...>> : IndexOfFromTypes<TargetTypeT, TypesT...>
{
};

template<typename TargetTypeT, typename TypeListT>
constexpr size_t IndexOfFromTypeListV{IndexOfFromTypeList<TargetTypeT, TypeListT>::Value};

} // namespace CPPS
