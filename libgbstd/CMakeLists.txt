

set(SOURCES
  math.cpp
  random.cpp
  sha256.cpp
  sns/sns_record.cpp
  sns/sns_record_table.cpp
  sns/sns_platform.cpp
  sns/sns_article.cpp
  sns/sns_account.cpp
  sns/sns_account_observer.cpp
  sns/sns_timeline.cpp
  sns/sns_timeline_node.cpp
  sns/sns_tester.cpp
  sns/sns_index_list.cpp
  parsers/token.cpp
  parsers/token_block.cpp
  parsers/tokenizer.cpp
  parsers/tokenizer__skip.cpp
  parsers/tokenizer__read_number.cpp
  characters/unicode.cpp
  characters/font.cpp
  characters/text.cpp
  characters/string_set.cpp
  images/image.cpp
  images/image__png.cpp
  images/canvas.cpp
  images/canvas__draw.cpp
  images/canvas__draw_string.cpp
  images/liner.cpp
  tile_maps/tile_source.cpp
  tile_maps/tile_map.cpp
  tile_maps/tile_map_view.cpp
  values/object.cpp
  values/array.cpp
  values/object.cpp
  menus/table.cpp
  menus/view.cpp
  menus/basic_menu_window.cpp
  menus/stack.cpp
  menus/object.cpp
  widgets/operating_node.cpp
  widgets/node.cpp
  widgets/node_iterator.cpp
  widgets/label.cpp
  widgets/button.cpp
  widgets/frame.cpp
  widgets/icon.cpp
  widgets/iconshow.cpp
  widgets/dial.cpp
  widgets/checkbox.cpp
  widgets/radio_button.cpp
  widgets/menu_handler.cpp
  widgets/slider.cpp
  file_ops/file_op.cpp
  file_ops/riff_chunk.cpp
  file_ops/riff_subchunk_view.cpp
  file_ops/wave.cpp
  file_ops/wave_format.cpp
  file_ops/ogg_page.cpp
  sounds/sound_device.cpp
  sounds/sine_wave_device.cpp
  sounds/square_wave_device.cpp
  sounds/triangle_wave_device.cpp
  sounds/sawtooth_wave_device.cpp
  sounds/noise_device.cpp
  sounds/short_noise_device.cpp
  sounds/scale_table.cpp
  processes/timer.cpp
  processes/clock.cpp
  processes/task.cpp
  processes/process.cpp
  utility.cpp
  control.cpp
)


include_directories(./)
include_directories(../)
add_library("gbstd" ${SOURCES})




