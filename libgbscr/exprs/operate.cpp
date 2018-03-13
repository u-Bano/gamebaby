#include"libgbscr/expr.hpp"
#include"libgbscr/process.hpp"
#include<new>


namespace gbscr{
namespace exprs{




void
operate_prefix_unary(operand&  o, operator_word  opw, process*  proc) noexcept
{
  auto  i = o.evaluate(proc).get_integer_safely();

    if(opw == gbstd::string_view("!"))
    {
      o = operand(value(!i));
    }

  else
    if(opw == gbstd::string_view("~"))
    {
      o = operand(value(~i));
    }

  else
    if(opw == gbstd::string_view("-"))
    {
      o = operand(value(-i));
    }
}


void
operate_postfix_unary(operand&  o, operator_word  opw, process*  proc) noexcept
{
    if(opw == gbstd::string_view(""))
    {
    }
}


void
operate_binary(operand&  lo, operand&  ro, operator_word  opw, process*  proc) noexcept
{
  auto  lv = lo.evaluate(proc);

    if(opw == gbstd::string_view("||"))
    {
        if(lv.get_integer_safely())
        {
          lo = operand(value(true));
        }

      else
        {
          lo = operand(value(ro.evaluate(proc).get_integer_safely()));
        }


      return;
    }

  else
    if(opw == gbstd::string_view("&&"))
    {
        if(lv.get_integer_safely())
        {
          lo = operand(value(ro.evaluate(proc).get_integer_safely()));
        }

      else
        {
          lo = operand(value(false));
        }


      return;
    }



    if(opw == gbstd::string_view("."))
    {
        if(!ro.is_identifier())
        {
          printf("右辺が識別子ではない\n");
        }

      else
        if(!lv.is_reference())
        {
          printf(".%s\n",ro.get_string().data());

          printf("左辺が参照ではない\n");

          lv.print();
        }

      else
        {
          lv = lv.get_reference()().get_value();

/*
            if(!lv.is_object())
            {
              printf("左辺がオブジェクトではない\n");

              lv.print();
            }

          else
            {
              auto&  obj = lv.get_object();

              auto&  s = ro.get_string();

              lo = operand(value(obj[s.view()]));
            }
*/
        }


      return;
    }

  else
    if(opw == gbstd::string_view("->"))
    {
      printf("->演算は未実装\n");

      return;
    }

  else
    if(opw == gbstd::string_view("?"))
    {
        if(!ro.is_paired_expression())
        {
          printf("右辺がpaired_expressionではない\n");
        }

      else
        {
          auto  cond = lv.get_integer_safely();

          auto&  pe = ro.get_paired_expression();

          auto  result = (cond? pe.get_left():pe.get_right()).evaluate(proc);

          lo = operand(std::move(result));
        }


      return;
    }

  else
    if(opw == gbstd::string_view("()"))
    {
        if(!ro.is_expression_list())
        {
          printf("右辺がexpression_listではない\n");

          return;
        }


      auto&  exls = ro.get_expression_list();

        if(lv.is_reference())
        {
          lv = lv.get_reference()().get_value();
        }


      lo = operand(value());

        if(lv.is_routine())
        {
          proc->prepare_call(lv.get_routine(),exls,lo.get_value_pointer());
        }
/*
      else
        if(lv.is_method_calling())
        {
          proc->prepare_call(lv.get_method_calling(),exls,lo.get_value_pointer());
        }
*/

      else
        {
          printf("左辺がroutineでも、メソッド呼び出しでもない\n");

          lv.print();
        }


      return;
    }

  else
    if(opw == gbstd::string_view("[]"))
    {
      printf("[]演算は未実装\n");

      return;
    }



  auto  ri = ro.evaluate(proc).get_integer_safely();
  auto  li =               lv.get_integer_safely();

    if((opw == gbstd::string_view(  "=")) ||
       (opw == gbstd::string_view( "+=")) ||
       (opw == gbstd::string_view( "-=")) ||
       (opw == gbstd::string_view( "*=")) ||
       (opw == gbstd::string_view( "/=")) ||
       (opw == gbstd::string_view( "%=")) ||
       (opw == gbstd::string_view("<<=")) ||
       (opw == gbstd::string_view(">>=")) ||
       (opw == gbstd::string_view( "|=")) ||
       (opw == gbstd::string_view( "&=")) ||
       (opw == gbstd::string_view( "^=")))
    {
        if(lv.is_reference())
        {
          auto&  objv = lv.get_reference()().get_value();

               if(opw == gbstd::string_view(  "=")){objv = value(   ri);}
          else if(opw == gbstd::string_view( "+=")){objv = value(li+ri);}
          else if(opw == gbstd::string_view( "-=")){objv = value(li-ri);}
          else if(opw == gbstd::string_view( "*=")){objv = value(li*ri);}
          else if(opw == gbstd::string_view( "/=")){objv = value(li/ri);}
          else if(opw == gbstd::string_view( "%=")){objv = value(li%ri);}
          else if(opw == gbstd::string_view("<<=")){objv = value(li<<ri);}
          else if(opw == gbstd::string_view(">>=")){objv = value(li>>ri);}
          else if(opw == gbstd::string_view( "|=")){objv = value(li|ri);}
          else if(opw == gbstd::string_view( "&=")){objv = value(li&ri);}
          else if(opw == gbstd::string_view( "^=")){objv = value(li^ri);}
          else {printf("参照に対する不正な演算\n");}
        }


      return;
    }




       if(opw == gbstd::string_view("+")){lo = operand(value(li+ri));}
  else if(opw == gbstd::string_view("-")){lo = operand(value(li-ri));}
  else if(opw == gbstd::string_view("*")){lo = operand(value(li*ri));}
  else
    if(opw == gbstd::string_view("/"))
    {
        if(!ri)
        {
          printf("div error: ゼロ除算\n");
        }

      else
        {
          lo = operand(value(li/ri));
        }
    }

  else
    if(opw == gbstd::string_view("%"))
    {
        if(!ri)
        {
          printf("rem error: ゼロ除算\n");
        }

      else
        {
          lo = operand(value(li%ri));
        }
    }

  else if(opw == gbstd::string_view("<<")){lo = operand(value(li<<ri));}
  else if(opw == gbstd::string_view(">>")){lo = operand(value(li>>ri));}
  else if(opw == gbstd::string_view("|")) {lo = operand(value(li|ri));}
  else if(opw == gbstd::string_view("&")) {lo = operand(value(li&ri));}
  else if(opw == gbstd::string_view("^")) {lo = operand(value(li^ri));}
  else if(opw == gbstd::string_view("==")){lo = operand(value(li == ri));}
  else if(opw == gbstd::string_view("!=")){lo = operand(value(li != ri));}
  else if(opw == gbstd::string_view("<")) {lo = operand(value(li <  ri));}
  else if(opw == gbstd::string_view("<=")){lo = operand(value(li <= ri));}
  else if(opw == gbstd::string_view(">")) {lo = operand(value(li >  ri));}
  else if(opw == gbstd::string_view(">=")){lo = operand(value(li >= ri));}
  else
    if(opw == gbstd::string_view("::"))
    {
      printf("::演算は未実装\n");
    }

  else
    if(opw == gbstd::string_view(","))
    {
      lo = ro;
    }
}


void
operate_conditional(operand&  o1, operand&  o2, operand&  o3, process*  proc) noexcept
{
  auto  cond = o1.evaluate(proc).get_integer_safely();

  o1 = operand((cond? o2:o3).evaluate(proc));
}


void
operate_stack(operand_stack&  stack, const expr_element&  e, process*  proc) noexcept
{
    if(e.is_operand())
    {
      stack.push(e.get_operand());
    }

  else
    if(e.is_prefix_unary_operator())
    {
        if(stack.size() < 1)
        {
          printf("単項演算の演算項が足りない\n");

          return;
        }


      operate_prefix_unary(stack.top(),e.get_operator_word(),proc);
    }

  else
    if(e.is_postfix_unary_operator())
    {
        if(stack.size() < 1)
        {
          printf("単項演算の演算項が足りない\n");

          return;
        }


      operate_postfix_unary(stack.top(),e.get_operator_word(),proc);
    }

  else
    if(e.is_binary_operator())
    {
        if(stack.size() < 2)
        {
          printf("二項演算の演算項が足りない\n");

          return;
        }


      auto&  ro = stack.pop();
      auto&  lo = stack.top();

      operate_binary(lo,ro,e.get_operator_word(),proc);
    }
}




}}




