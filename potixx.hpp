//! \file potixx.hpp
//! \brief A simple C++ wrapper classes for Poti(https://github.com/schnorr/poti).

#ifndef POTIXX_HPP
#define POTIXX_HPP

#include <cassert>
#include <string>
#include <vector>

#include <poti.h>

namespace poti {
  struct StateType;
  struct LinkType;

  struct ContainerType {
    std::string name;
    ContainerType *parent;

    ContainerType(const char *nm, ContainerType* prnt = NULL)
      : name(nm), parent(prnt)
    {
      assert(name.size() > 0);
      
      poti_DefineContainerType(name.c_str(),
                               (parent == NULL) ? "0" : parent->name.c_str(),
                               name.c_str());
    }
  };

  struct StateType {
    std::string name;
    ContainerType *parent;
    StateType(const char *nm, ContainerType *prnt) : name(nm), parent(prnt) {
      assert(name.size() > 0);
      poti_DefineStateType(name.c_str(), parent->name.c_str(), name.c_str());
    };
  };

  struct LinkType {
    std::string name;
    ContainerType *parent;
    ContainerType *start;
    ContainerType *end;

  public:
    LinkType(const char *nm, ContainerType *p, ContainerType *s, ContainerType *e)
      : name(nm), parent(p), start(s), end(e)
    {
      assert(name.size() > 0);
      poti_DefineLinkType(name.c_str(), p->name.c_str(), s->name.c_str(), e->name.c_str(), name.c_str()); 
    }
  };

  template<class ParentType>
  struct EntityValue {
    std::string name;
    ParentType *type;
    double red, green, blue;

    EntityValue(const char *nm, ParentType *t, double r, double g, double b)
      : name(nm), type(t), red(r), green(g), blue(g)
    {
      assert(type);
      char buf[100];
      snprintf(buf, 100, "%.1f %.1f %.1f", red, green, blue);
      poti_DefineEntityValue(name.c_str(), type->name.c_str(), name.c_str(), buf);
    }
  };

  struct Container {
    double start;
    std::string name;
    ContainerType *type;
    Container *parent;
    std::vector<EntityValue<StateType>*> state_stack;

    Container(double s, const char *n, ContainerType *t, Container *p)
      : start(s), name(n), type(t), parent(p), state_stack()
    {
      assert(name.size() > 0);
      assert(type);

      poti_CreateContainer(start, name.c_str(), type->name.c_str(),
                           (parent ? parent->name.c_str() : "0"),
                           name.c_str());
    }

    void destroy(double s) {
      poti_DestroyContainer(s, type->name.c_str(), name.c_str());
    }

    void pushState(double s, EntityValue<StateType> *value) {
      poti_PushState(s, name.c_str(), value->type->name.c_str(), value->name.c_str());
      state_stack.push_back(value);
    }

    void popState(double s) {
      assert(state_stack.size() > 0);
      EntityValue<StateType> *value = state_stack[state_stack.size() - 1];
      state_stack.pop_back();
      poti_PopState(s, name.c_str(), value->type->name.c_str());
    }
  };

  struct Link {
    Link(LinkType *type, Container *parent,
         Container *sc, double st, // start container, start time
         Container *ec, double et, // end container, end time
         EntityValue<LinkType> *entity, const char *key)
    {
      poti_StartLink(st, parent->name.c_str(), type->name.c_str(),
                     sc->name.c_str(), entity->name.c_str(), key);
      poti_EndLink(et, parent->name.c_str(), type->name.c_str(),
                   ec->name.c_str(), entity->name.c_str(), key);
    }
  };
}

#endif // POTIXX_HPP
