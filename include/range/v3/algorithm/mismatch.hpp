/// \file
// Range v3 library
//
//  Copyright Eric Niebler 2014-present
//
//  Use, modification and distribution is subject to the
//  Boost Software License, Version 1.0. (See accompanying
//  file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt)
//
// Project home: https://github.com/ericniebler/range-v3
//
#ifndef RANGES_V3_ALGORITHM_MISMATCH_HPP
#define RANGES_V3_ALGORITHM_MISMATCH_HPP

#include <utility>
#include <meta/meta.hpp>
#include <range/v3/range_fwd.hpp>
#include <range/v3/range/access.hpp>
#include <range/v3/range/concepts.hpp>
#include <range/v3/range/dangling.hpp>
#include <range/v3/range/traits.hpp>
#include <range/v3/algorithm/result_types.hpp>
#include <range/v3/functional/comparisons.hpp>
#include <range/v3/functional/identity.hpp>
#include <range/v3/functional/invoke.hpp>
#include <range/v3/iterator/concepts.hpp>
#include <range/v3/iterator/traits.hpp>
#include <range/v3/utility/static_const.hpp>

namespace ranges
{
    /// \addtogroup group-algorithms
    /// @{
    template<typename I1, typename I2>
    using mismatch_result = detail::in1_in2_result<I1, I2>;

    struct mismatch_fn
    {
        CPP_template(typename I1, typename S1, typename I2, typename C = equal_to,
            typename P1 = identity, typename P2 = identity)(
            requires InputIterator<I1> && Sentinel<S1, I1> && InputIterator<I2> &&
                IndirectRelation<C, projected<I1, P1>, projected<I2, P2>>)
        RANGES_EMPTY_ATTR
        RANGES_DEPRECATED("Use the variant of ranges::mismatch that takes an upper bound for "
            "both sequences")
        mismatch_result<I1, I2> operator()(I1 begin1, S1 end1, I2 begin2, C pred = C{}, P1 proj1 = P1{},
                P2 proj2 = P2{}) const
        {
            for(; begin1 != end1; ++begin1, ++begin2)
                if(!invoke(pred, invoke(proj1, *begin1), invoke(proj2, *begin2)))
                    break;
            return {begin1, begin2};
        }

        CPP_template(typename I1, typename S1, typename I2, typename S2, typename C = equal_to,
            typename P1 = identity, typename P2 = identity)(
            requires InputIterator<I1> && Sentinel<S1, I1> &&
                InputIterator<I2> && Sentinel<S2, I2> &&
                IndirectRelation<C, projected<I1, P1>, projected<I2, P2>>)
        mismatch_result<I1, I2> operator()(I1 begin1, S1 end1, I2 begin2, S2 end2, C pred = C{}, P1 proj1 = P1{},
                P2 proj2 = P2{}) const
        {
            for(; begin1 != end1 &&  begin2 != end2; ++begin1, ++begin2)
                if(!invoke(pred, invoke(proj1, *begin1), invoke(proj2, *begin2)))
                    break;
            return {begin1, begin2};
        }

        CPP_template(typename Rng1, typename I2Ref, typename C = equal_to, typename P1 = identity,
            typename P2 = identity)(
            requires InputRange<Rng1> && InputIterator<uncvref_t<I2Ref>> &&
                IndirectRelation<C, projected<iterator_t<Rng1>, P1>,
                                    projected<uncvref_t<I2Ref>, P2>>)
        RANGES_EMPTY_ATTR
        RANGES_DEPRECATED("Use the variant of ranges::mismatch that takes an upper bound for "
            "both sequences")
        mismatch_result<safe_iterator_t<Rng1>, uncvref_t<I2Ref>>
        operator()(Rng1 &&rng1, I2Ref &&begin2, C pred = C{}, // see below [*]
                P1 proj1 = P1{}, P2 proj2 = P2{}) const
        {
            RANGES_DIAGNOSTIC_PUSH
            RANGES_DIAGNOSTIC_IGNORE_DEPRECATED_DECLARATIONS
            return (*this)(begin(rng1), end(rng1), static_cast<uncvref_t<I2Ref> &&>(begin2), std::move(pred),
                std::move(proj1), std::move(proj2));
            RANGES_DIAGNOSTIC_POP
        }

        CPP_template(typename Rng1, typename Rng2, typename C = equal_to, typename P1 = identity,
            typename P2 = identity)(
            requires InputRange<Rng1> && InputRange<Rng2> &&
                IndirectRelation<C, projected<iterator_t<Rng1>, P1>,
                                    projected<iterator_t<Rng2>, P2>>)
        mismatch_result<safe_iterator_t<Rng1>, safe_iterator_t<Rng2>>
        operator()(Rng1 &&rng1, Rng2 &&rng2, C pred = C{}, P1 proj1 = P1{},
                P2 proj2 = P2{}) const
        {
            return (*this)(begin(rng1), end(rng1), begin(rng2), end(rng2), std::move(pred),
                std::move(proj1), std::move(proj2));
        }
    };

    /// \sa `mismatch_fn`
    /// \ingroup group-algorithms
    RANGES_INLINE_VARIABLE(mismatch_fn, mismatch)

    // [*] In this case, the 'begin2' iterator is taken by universal reference. Why? So
    // that we can properly distinguish this case:
    //   int x[] = {1,2,3,4};
    //   int y[] = {1,2,3,4};
    //   mismatch(x, y);
    // Had 'begin2' been taken by value as is customary, this call could match as either
    // two ranges, or a range and an iterator, where the iterator is the array, decayed
    // to a pointer. Yuk!

    /// @}
} // namespace ranges

#endif // include guard
