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
#ifndef RANGES_V3_ALGORITHM_REPLACE_HPP
#define RANGES_V3_ALGORITHM_REPLACE_HPP

#include <meta/meta.hpp>
#include <range/v3/range_fwd.hpp>
#include <range/v3/range/access.hpp>
#include <range/v3/range/concepts.hpp>
#include <range/v3/range/dangling.hpp>
#include <range/v3/range/traits.hpp>
#include <range/v3/functional/identity.hpp>
#include <range/v3/functional/invoke.hpp>
#include <range/v3/iterator/concepts.hpp>
#include <range/v3/iterator/traits.hpp>
#include <range/v3/utility/static_const.hpp>

namespace ranges
{
    /// \addtogroup group-algorithms
    /// @{
    struct replace_fn
    {
        CPP_template(typename I, typename S, typename T1, typename T2, typename P = identity)(
            requires InputIterator<I> && Sentinel<S, I> &&
                Writable<I, T2 const &> &&
                IndirectRelation<equal_to, projected<I, P>, T1 const *>)
        I operator()(I begin, S end, T1 const & old_value, T2 const & new_value, P proj = {}) const
        {
            for(; begin != end; ++begin)
                if(invoke(proj, *begin) == old_value)
                    *begin = new_value;
            return begin;
        }

        CPP_template(typename Rng, typename T1, typename T2, typename P = identity)(
            requires InputRange<Rng> &&
                Writable<iterator_t<Rng>, T2 const &> &&
                IndirectRelation<equal_to, projected<iterator_t<Rng>, P>, T1 const *>)
        safe_iterator_t<Rng> operator()(Rng &&rng, T1 const & old_value, T2 const & new_value, P proj = {}) const
        {
            return (*this)(begin(rng), end(rng), old_value, new_value, std::move(proj));
        }
    };

    /// \sa `replace_fn`
    /// \ingroup group-algorithms
    RANGES_INLINE_VARIABLE(replace_fn, replace)
    /// @}
} // namespace ranges

#endif // include guard
