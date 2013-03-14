#pragma once

#include <boost/mpl/bind.hpp>
#include <boost/mpl/lambda.hpp>
#include <boost/mpl/inherit.hpp>
#include <boost/mpl/empty_base.hpp>
#include <boost/mpl/inherit_linearly.hpp>

template <class Seq, class Unit>
struct GenScatterHierarchy
  : boost::mpl::inherit_linearly<
      Seq,
      boost::mpl::inherit<
        boost::mpl::placeholders::_1,
        boost::mpl::bind<
          typename boost::mpl::lambda<Unit>::type,
          boost::mpl::placeholders::_2
        >
      >
    >
{ };