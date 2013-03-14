#pragma once

namespace atlas{
    
    template<class T>
    class Singleton{
        Singleton();
        ~Singleton();

    public:
        static T& instance(){
            static T object;
            return object;
        }
    };

} //namespace atlas