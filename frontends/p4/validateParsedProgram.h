/*
Copyright 2013-present Barefoot Networks, Inc.

Licensed under the Apache License, Version 2.0 (the "License");
you may not use this file except in compliance with the License.
You may obtain a copy of the License at

    http://www.apache.org/licenses/LICENSE-2.0

Unless required by applicable law or agreed to in writing, software
distributed under the License is distributed on an "AS IS" BASIS,
WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
See the License for the specific language governing permissions and
limitations under the License.
*/

#ifndef _P4_VALIDATEPARSEDPROGRAM_H_
#define _P4_VALIDATEPARSEDPROGRAM_H_

#include "ir/ir.h"

namespace P4 {

/*
   Run immediately after parsing. There is no type information at this
   point, so we do only simple checks.
   - integer constants have valid types
   - don't care _ is not used as a name for methods, fields, variables, instances
   - unions have at least one field
   - width of bit<> types is positive
   - width of int<> types is larger than 1
   - no parser state is named 'accept' or 'reject'
   - constructor parameters are direction-less
   - tables have an actions and a default_action properties
   - table parameters are never directionless
   - instantiations appear at the top-level only
   - switch statements do not occur in actions
   - instantiations do not occur in actions
   - constructors are not invoked in actions
 */
class ValidateParsedProgram final : public Inspector {
    bool isv1;
    void container(const IR::IContainer* type);

 public:
    explicit ValidateParsedProgram(bool isv1) : isv1(isv1)
    { setName("ValidateParsedProgram"); }
    void postorder(const IR::Constant* c) override;
    void postorder(const IR::SwitchStatement* statement) override;
    void postorder(const IR::Method* t) override;
    void postorder(const IR::StructField* f) override;
    void postorder(const IR::ParserState* s) override;
    void postorder(const IR::P4Table* t) override;
    void postorder(const IR::Type_Union* type) override;
    void postorder(const IR::Type_Bits* type) override;
    void postorder(const IR::ConstructorCallExpression* expression) override;
    void postorder(const IR::Declaration_Variable* decl) override;
    void postorder(const IR::Declaration_Instance* inst) override;
    void postorder(const IR::Declaration_Constant* decl) override;
    void postorder(const IR::ReturnStatement* statement) override;
    void postorder(const IR::ExitStatement* statement) override;
    void postorder(const IR::Type_Package* package) override
    { container(package); }
    void postorder(const IR::P4Control* control) override
    { container(control); }
    void postorder(const IR::P4Parser* parser) override
    { container(parser); }
};

}  // namespace P4

#endif /* _P4_VALIDATEPARSEDPROGRAM_H_ */
