#include"utility.hpp"
#include"string.hpp"
#include<cstdio>




namespace gbstd{


int  reporting_counter;


gbstd::string
make_string_from_file(const char*  filepath) noexcept
{
  gbstd::string  s;

  auto  f = fopen(filepath,"rb");

    if(f)
    {
        for(;;)
        {
          auto  c = fgetc(f);

            if(feof(f))
            {
              break;
            }


          s.append(c);
        }


      fclose(f);
    }


  return std::move(s);
}


void
printf_with_indent(int  indent, const char*  fmt, ...) noexcept
{
  char  buf[256];

  va_list  ap;
  va_start(ap,fmt);

  vsnprintf(buf,sizeof(buf),fmt,ap);

  va_end(ap);

  const char*  p = buf;

    while(*p)
    {
      fputc(*p,stdout);

        if(*p++ == '\n')
        {
            for(int  n = 0;  n < indent;  ++n)
            {
              fputc(' ',stdout);
            }
        }
    }
}


std::random_device
random_device;


std::default_random_engine
random_number_engine;


}




