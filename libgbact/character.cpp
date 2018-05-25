#include"libgbact/character.hpp"




namespace gbact{
namespace characters{


images::image
g_image;




void
character::
set_data(character_data*  dat) noexcept
{
  delete m_data      ;
         m_data = dat;

    if(m_data)
    {
      m_data->set_character(*this);

      m_data->initialize();
    }
}


void
character::
do_when_collided(object&  other_side, spaces::position  position) noexcept
{
    if(other_side.get_kind_code() == kind_codes::player)
    {
      auto&  chr = static_cast<character&>(other_side);

      m_data->do_when_collided_with_player(static_cast<player&>(*chr.m_data),position);
    }

  else
    if(other_side.get_kind_code() == kind_codes::bullet)
    {
      auto&  chr = static_cast<character&>(other_side);

      m_data->do_when_collided_with_bullet(static_cast<bullet&>(*chr.m_data),position);
    }

  else
    {
      m_data->do_when_collided_with_object(other_side,position);
    }
}


void
character::
update() noexcept
{
    if(m_data)
    {
      m_data->update_parameter();
      m_data->update_image();
    }


  object::update();
}


void
character::
render(images::image&  dst) const noexcept
{
  image_object::render(dst);


  auto  rect = get_rectangle();

  dst.draw_rectangle_safely(colors::red,rect.x,rect.y,rect.w,rect.h);
}




}}




