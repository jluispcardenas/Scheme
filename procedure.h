/************************************************************************
  Scheme interpreter
  
  Copyright (C) 2010 JLPC

************************************************************************/

#ifndef _PROCEDURE_H_
#define _PROCEDURE_H_

#include <string>
#include "environment.h"
#include "object.h"

class procedure : public object
{
      public:
      procedure(std::string id, object* code, environment* env) {
              this->id = id;
              this->code = code;
              this->env = env;
      }
      virtual object* invoke(std::list<object*> args, environment* env) = 0;
        
      protected:
      std::string id;
      object* code;
      environment* env;
};

#endif
