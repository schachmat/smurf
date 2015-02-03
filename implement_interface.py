#!/bin/env python

import sys
from pycparser import parse_file, c_generator, c_ast

sys.path.extend(['.', '..'])


def impl_struct():
    return True


def impl_func():
    return True


class StructVisitor(c_ast.NodeVisitor):
    def visit_Struct(self, node):
        try:
            if not 'cef_base_t' in node.decls[0].type.type.names:
                return
        except AttributeError:
            return
        except TypeError:
            return

        sname = node.name
        fv = FuncDeclVisitor(sname[5:-1])
        fv.visit(node)

        hname = node.coord

        ret = []
        ret.append('/**********************************************************')
        ret.append(' * BEWARE: This file is a autogenerated stub!')
        ret.append(' *********************************************************/')
        ret.append('')
        ret.append('#include <stdlib.h>')
        ret.append('')
        ret.append('#include "' + str(hname).lstrip('./').rstrip(':0123456789') + '"')
        ret.append('')
        ret.append('#include "cef/base.h"')
        ret.append('#include "util.h"')
        ret.append('')
        ret.append('\n'.join(fv.ret))
        ret.append('struct ' + sname + ' *init_' + sname[5:-2] + '()')
        ret.append('{')
        ret.append('\tstruct ' + sname + ' *ret = NULL;')
        ret.append('\tstruct refcount *r = NULL;')
        ret.append('\tchar *cp = NULL;')
        ret.append('')
        ret.append('\tDEBUG_ONCE("init_' + sname[5:-2] + '() called");')
        ret.append('\tif (!(r = calloc(sizeof(struct refcount) + sizeof(struct ' + sname + '), 1))) {')
        ret.append('\t\teprintf("out of memory");')
        ret.append('\t\treturn NULL;')
        ret.append('\t}')
        ret.append('')
        ret.append('\tcp = (char*)r;')
        ret.append('\tcp += sizeof(struct refcount);')
        ret.append('\tret = (struct ' + sname + '*)cp;')
        ret.append('')
        ret.append('\tif(!init_base((cef_base_t*)ret, sizeof(struct ' + sname + '))) {')
        ret.append('\t\tfree(r);')
        ret.append('\t\treturn NULL;')
        ret.append('\t}')
        ret.append('\tret->base.add_ref((cef_base_t*)ret);')
        ret.append('')
        ret.append('\t// callbacks')
        ret.append(fv.get_funlist())
        ret.append('')
        ret.append('\treturn ret;')
        ret.append('}')

        with open('cef/stubs/' + sname[5:-2] + '.c', 'w') as fh:
            fh.write('\n'.join(ret).replace('  ', '\t').replace('\n\n\n', '\n\n'))
#        print('\n'.join(ret).replace('  ', '\t').replace('\n\n\n', '\n\n'))


class FuncDeclVisitor(c_ast.NodeVisitor):
    def __init__(self, sname):
        self.sname = sname
        self.gen = c_generator.CGenerator()
        self.funs = {}
        self.ret = []

    def get_funlist(self):
        return '\n'.join(['\tret->' + m + ' = &' + f + ';' for m, f in self.funs.items()])

    def visit_FuncDecl(self, node):
        try:
            mfun = node.type.declname
            fun = self.sname + mfun
            node.type.declname = fun
            rett = ' '.join(node.type.type.names)
        except AttributeError: # This means the function returns a pointer
            mfun = node.type.type.declname
            fun = self.sname + mfun
            node.type.type.declname = fun
            rett = None
        param = c_ast.Constant("string", '"' + fun + '() called"')
        elist = c_ast.ExprList([param])
        call = c_ast.FuncCall(c_ast.ID('DEBUG_ONCE'), elist)

        compound = c_ast.Compound([call])

        rval = {
            'int': c_ast.Constant("int", "0"),
            'int32': c_ast.Constant("int", "0"),
            'int64': c_ast.Constant("int", "0"),
            'uint32': c_ast.Constant("int", "0"),
            'size_t': c_ast.Constant("int", "0"),
            'double': c_ast.Constant("int", "0"),
            'unsigned long': c_ast.Constant("int", "0"),
            # Enums...
            'cef_color_model_t': c_ast.Constant("int", "0"),
            'cef_context_menu_media_state_flags_t': c_ast.Constant("int", "0"),
            'cef_context_menu_media_type_t': c_ast.Constant("int", "0"),
            'cef_context_menu_type_flags_t': c_ast.Constant("int", "0"),
            'cef_context_menu_edit_state_flags_t': c_ast.Constant("int", "0"),
            'cef_dom_document_type_t': c_ast.Constant("int", "0"),
            'cef_dom_node_type_t': c_ast.Constant("int", "0"),
            'cef_duplex_mode_t': c_ast.Constant("int", "0"),
            'cef_errorcode_t': c_ast.Constant("int", "0"),
            'cef_menu_item_type_t': c_ast.Constant("int", "0"),
            'cef_postdataelement_type_t': c_ast.Constant("int", "0"),
            'cef_resource_type_t': c_ast.Constant("int", "0"),
            'cef_transition_type_t': c_ast.Constant("int", "0"),
            'cef_urlrequest_status_t': c_ast.Constant("int", "0"),
            'cef_value_type_t': c_ast.Constant("int", "0"),
            'cef_xml_node_type_t': c_ast.Constant("int", "0"),
            # special cases...
            'time_t': c_ast.Constant("int", "0"),
            'cef_time_t': c_ast.Constant("Struct", "(cef_time_t){}"),
            'cef_string_userfree_t': c_ast.Constant("Ptr", "NULL"),
            None: c_ast.Constant("Ptr", "NULL"),
        }.get(rett, None)
        if not rval is None:
            ret = c_ast.Return(rval)
            compound.block_items.append(ret)
        elif rett != 'void':
            print('Return type not handled: ' + rett)
            sys.exit(1)

        decl = c_ast.Decl(fun, None, None, ["CEF_CALLBACK"], node, None, None)
        fdef = c_ast.FuncDef(decl, None, compound)

        self.funs[mfun] = fun
        # fix const double pointer: https://github.com/eliben/pycparser/issues/68
        self.ret.append(self.gen.visit(fdef).replace('* const ', ' const* '))
#        fdef.show()


if __name__ == "__main__":
    if len(sys.argv) != 2:
        sys.exit(1)

    ast = parse_file(sys.argv[1], use_cpp=True, cpp_args=[r'-Ifake', r'-I.'])

    ast.show()
    StructVisitor().visit(ast)
