//
// Created by core on 2/9/20.
//

#pragma once

/**
 * \brief Uses a given combining operation, recombines the results of recursively processing its constituent parts, building up a return value.
 * \tparam Y return type
 * \tparam F type of combining function
 * \tparam Xs type argument parameter pack
 * \param n neutral element
 * \param f combining function
 * \param xs argument parameter pack
 * \return result
 */
template<typename Y, typename F, typename, typename ... Xs>
Y foldt(Y n, F f, Xs... xs);

template<typename Y, typename F>
Y foldt(Y n, F)
{
	return n;
}

template<typename Y, typename F, typename X, typename ... Xs>
Y foldt(Y n, F f, X x, Xs... xs)
{
	return foldt(f(n, x), f, xs...);
}
