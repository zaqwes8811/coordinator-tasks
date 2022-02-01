#include <boost/python.hpp>
using namespace boost::python;
#include <iostream>
struct World
{
    World(std::string msg_): msg(msg_) {}
    std::string greet() 
    { 
        return msg; 
    }
    std::string msg;
};

/// ///
BOOST_PYTHON_MODULE(mfw)
{
    class_<World>("World", init<std::string>())
        .def("greet", &World::greet)
    ;
}