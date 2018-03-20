#include"libgbscr/value.hpp"
#include"libgbscr/expr.hpp"


namespace gbscr{
namespace values{




void
variable::
set_carry_flag() noexcept
{
    if(!m_carry_flag)
    {
      m_carry_flag = true;

      auto&  v = m_value.is_reference()? m_value.get_reference()->get_value()
                                       : m_value;

        if(v.is_table())
        {
          auto&  obs = v.get_table_observer();

            if(obs)
            {
              obs->set_carry_flag();
            }
        }
    }
}


void
variable::
unset_carry_flag() noexcept
{
  m_table = nullptr;

  m_carry_flag = false;

  m_name = "";
}


void
variable::
print() const noexcept
{
  printf("%s:",m_name.data());

  m_value.print();
}




bool
read_variable(cursor&  cur, variable*&  var, process&  proc)
{
    if(!cur->is_identifier() &&
       !cur->is_string())
    {
      printf("テーブルの要素は、識別子がなくてはならない\n");

      return false;
    }


  auto  name = cur++->get_string().view();

    if(!cur->is_punctuations(":"))
    {
      printf("テーブルの要素は、識別子の後の値は\':\'で区切られなくてはならない\n");

      return false;
    }


  ++cur;

  operand  o;

    if(read_operand(cur,o,proc))
    {
      var = variable::create_instance();

//      var->set_table(&tbl);
//      var->set_value(o.evaluate());
      var->set_name(name);

      return true;
    }


  return false;
}




}}




