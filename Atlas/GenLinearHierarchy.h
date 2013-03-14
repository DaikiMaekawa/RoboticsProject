#pragma once

#include <boost/mpl/bind.hpp>
#include <boost/mpl/lambda.hpp>
#include <boost/mpl/empty_base.hpp>
#include <boost/mpl/inherit_linearly.hpp>

template <class Seq, class Unit, class Root = boost::mpl::empty_base>
struct GenLinearHierarchy
  : boost::mpl::inherit_linearly<
      Seq,
      boost::mpl::bind<
        typename boost::mpl::lambda<Unit>::type,
        boost::mpl::placeholders::_2,
        boost::mpl::placeholders::_1
      >,
      Root
    >
{ };

