#pragma once

#include <boost/type.hpp>
#include <boost/mpl/apply.hpp>
#include <boost/mpl/find_if.hpp>
#include <boost/mpl/deref.hpp>
#include <boost/mpl/transform_view.hpp>
#include <boost/mpl/placeholders.hpp>
#include <boost/type_traits/is_same.hpp>
#include <boost/mpl/pop_front.hpp>
#include <boost/mpl/front.hpp>
#include "GenLinearHierarchy.h"
#include "GenScatterHierarchy.h"
#include <boost/lambda/construct.hpp>

namespace AbstractFactory_
{
  template <class T>
  struct AbstractFactoryUnit_
  {
      virtual T* doCreate(boost::type<T>) = 0;
      //virtual boost::lambda::new_ptr<T> doCreate(boost::type<T>) = 0;
      virtual ~AbstractFactoryUnit_(){ }
  };

  typedef boost::mpl::quote1<AbstractFactoryUnit_> AbstractFactoryUnit;
}

namespace ConcreteFactory_
{
  template <class ConcreateProduct, class Base>
  struct OpNewFactoryUnit_
    : Base
  {
    typedef OpNewFactoryUnit_ type;
  private:
    typedef typename Base::productList baseProductList;
  protected:
    typedef typename boost::mpl::pop_front<baseProductList>::type productList;
  public:
    typedef typename boost::mpl::front<baseProductList>::type AbstractProduct;
    
    virtual ConcreateProduct* doCreate(boost::type<AbstractProduct>)
    {
      return new ConcreateProduct();
    }
    
    /*
    virtual boost::lambda::new_ptr<AbstractProduct> doCreate(boost::type<AbstractProduct>){
        return boost::lambda::new_ptr<AbstractProduct>();
    }
    */

  };

  typedef boost::mpl::quote2<OpNewFactoryUnit_> OpNewFactoryUnit;
}

template <
  class ProductList,
  class Unit = AbstractFactory_::AbstractFactoryUnit
>
struct AbstractFactory
  : GenScatterHierarchy<ProductList, Unit>::type
{
  typedef ProductList productList;
  template <class T>
  T* create()
  {
    typedef typename boost::mpl::apply<Unit, T>::type unitType;
    return static_cast<unitType&>(*this).doCreate(boost::type<T>());
  }

  /*
  boost::lambda::new_ptr<T> create(){
      typedef typename boost::mpl::apply<Unit, T>::type unitType;
      return static_cast<unitType&>(*this).doCreate(boost::type<T>());
  }
  */

};

template <
  class AbstractFact,
  class ConcreteProductList = typename AbstractFact::productList,
  class Creator = ConcreteFactory_::OpNewFactoryUnit
>
struct ConcreteFactory
  : GenLinearHierarchy<ConcreteProductList, Creator, AbstractFact>::type
{ };

