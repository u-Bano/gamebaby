#ifndef LIBGBSND_value_HPP
#define LIBGBSND_value_HPP


#include<cstdint>
#include<cstdio>
#include<memory>
#include<vector>
#include"libgbstd/string.hpp"
#include"libgbscr/shared_string.hpp"
#include"libgbscr/list.hpp"
#include"libgbscr/token.hpp"


namespace gbscr{


namespace stmts{
class stmt;
class stmt_list;
class routine;
}


namespace processes{
class process;
}


namespace values{




class table;
class variable;
class value;
class method;


class
reference
{
  variable*  m_pointer;

public:
  constexpr reference(variable*  var) noexcept: m_pointer( var){}
  constexpr reference(variable&  var) noexcept: m_pointer(&var){}

  constexpr bool  operator==(const reference&  rhs) const noexcept{return m_pointer == rhs.m_pointer;}
  constexpr bool  operator!=(const reference&  rhs) const noexcept{return m_pointer != rhs.m_pointer;}

  constexpr  operator bool() const noexcept{return m_pointer;}

  constexpr variable*  operator->() const noexcept{return  m_pointer;}
  constexpr variable&  operator *() const noexcept{return *m_pointer;}

};


struct
calling
{
  values::table*  table;

  const stmts::routine*  routine;

};


class
table
{
  struct private_data;

  private_data*  m_data=nullptr;

  void  unrefer() noexcept;

public:
  table() noexcept{}
  table(const block&  blk, processes::process&  proc){assign(blk,proc);}
  table(const table&   rhs) noexcept{*this = rhs;}
  table(      table&&  rhs) noexcept{*this = std::move(rhs);}
 ~table(){unrefer();}

  table&  operator=(const table&   rhs) noexcept;
  table&  operator=(      table&&  rhs) noexcept;

  reference  operator[](gbstd::string_view  name) const noexcept;

  reference  find(gbstd::string_view  name) const noexcept;

  void  clear() const noexcept;

  table&  assign(const block&  blk, processes::process&  proc);

  reference  append(const value&  v, gbstd::string_view  name) const noexcept;

  void  print() const noexcept;

};


struct
user_data
{
  void*  pointer;

};


struct
value_conversion_error
{
};


class
value
{
  enum class kind{
    null,
    integer,
    constant_string,
    string,
    reference,
    routine,
    table,
    calling,
    user_data,
    host_function,

  } m_kind=kind::null;

  union data{
    int              i;
    shared_string    s;
    table          tbl;
    reference        r;

    const gbstd::string*   cs;
    const stmts::routine*  rt;

    data(){}
   ~data(){}

  } m_data;

public:
  value() noexcept{}
  value(bool  b) noexcept{*this = b;}
  value(int  i) noexcept{*this = i;}
  value(const gbstd::string&  s) noexcept{*this = s;}
  value(const shared_string&  s) noexcept{*this = s;}
  value(const reference&  r) noexcept{*this = r;}
  value(const stmts::routine&  rt) noexcept{*this = rt;}
  value(const table&  tbl) noexcept{*this = tbl;}
  value(const value&   rhs) noexcept{*this = rhs;}
  value(      value&&  rhs) noexcept{*this = std::move(rhs);}
 ~value(){clear();}	

  value&  operator=(bool  b) noexcept;
  value&  operator=(int  i) noexcept;
  value&  operator=(const gbstd::string&  s) noexcept;
  value&  operator=(const shared_string&  s) noexcept;
  value&  operator=(const reference&  r) noexcept;
  value&  operator=(const stmts::routine&  rt) noexcept;
  value&  operator=(const table&  tbl) noexcept;
  value&  operator=(const value&   rhs) noexcept;
  value&  operator=(      value&&  rhs) noexcept;

  operator bool() const noexcept{return m_kind != kind::null;}

  void  clear() noexcept;

  bool  is_null()           const noexcept{return m_kind == kind::null;}
  bool  is_reference()      const noexcept{return m_kind == kind::reference;}
  bool  is_integer()        const noexcept{return m_kind == kind::integer;}
  bool  is_consatant_string() const noexcept{return m_kind == kind::constant_string;}
  bool  is_string()         const noexcept{return m_kind == kind::string;}
  bool  is_routine()        const noexcept{return m_kind == kind::routine;}
  bool  is_table()         const noexcept{return m_kind == kind::table;}

  int                    get_integer()        const noexcept{return m_data.i;}
  const gbstd::string&   get_constant_string() const noexcept{return *m_data.cs;}
  const shared_string&   get_string()         const noexcept{return m_data.s;}
  const reference&       get_reference()      const noexcept{return m_data.r;}
  const stmts::routine&  get_routine()        const noexcept{return *m_data.rt;}
  const table&           get_table()          const noexcept{return m_data.tbl;}

  int                    convert_to_integer()   const;
  shared_string          convert_to_string()    const;
  const stmts::routine&  convert_to_routine()   const;

  void  print() const noexcept;

};


class
value_list: public list<value>
{
public:
  using list::list;

};


class
variable
{
  const table*  m_table=nullptr;

  values::value  m_value;

  gbstd::string  m_name;

  variable() noexcept{}

public:
  variable(const variable&   rhs) noexcept=default;
  variable(      variable&&  rhs) noexcept=delete;

  variable&  operator=(const variable&   rhs) noexcept=default;
  variable&  operator=(      variable&&  rhs) noexcept=delete;

  static variable*  create_instance() noexcept{return new variable;}

  void          set_table(const table*  t)       noexcept{       m_table = t;}
  const table*  get_table(               ) const noexcept{return m_table    ;}

  void          set_value(const value&  v)       noexcept{       m_value = v;}
  const value&  get_value(               ) const noexcept{return m_value    ;}

  reference  get_reference() noexcept{return reference(*this);}

  void                  set_name(gbstd::string_view  name)       noexcept{       m_name = name;}
  const gbstd::string&  get_name(                        ) const noexcept{return m_name       ;}

  void  print() const noexcept;

};


bool  read_variable(cursor&  cur, variable*&  var, processes::process&  proc);


}


using values::reference;
using values::value;
using values::value_conversion_error;
using values::value_list;
using values::variable;
using values::table;
using values::calling;


}




#endif




