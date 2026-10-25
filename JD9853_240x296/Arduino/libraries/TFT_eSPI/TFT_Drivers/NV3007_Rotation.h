// NV3007 rotation support (based on ST7789 rotation)

  writecommand(TFT_MADCTL);
  rotation = m % 4;
  switch (rotation) {
    case 0: // Portrait
#ifdef CGRAM_OFFSET
      if (_init_width == 135)
      {
        colstart = 52;
        rowstart = 40;
      }
      else if(_init_height == 280)
      {
        colstart = 0;
        rowstart = 20;
      }
      else
      {
        colstart = 0;
        rowstart = 0;
      }
#endif
      // Apply column offset if defined for NV3007
#ifdef TFT_COLUMN_OFFSET
  colstart += TFT_COLUMN_OFFSET;
#endif
      writedata(TFT_MAD_COLOR_ORDER);

      _width  = _init_width;
      _height = _init_height;
      break;

    case 1: // Landscape (Portrait + 90)
#ifdef CGRAM_OFFSET
      if (_init_width == 135)
      {
        colstart = 40;
        rowstart = 53;
      }
      else if(_init_height == 280)
      {
        colstart = 20;
        rowstart = 0;
      }
      else
      {
        colstart = 0;
        rowstart = 0;
      }
#endif
#ifdef TFT_COLUMN_OFFSET
  colstart += TFT_COLUMN_OFFSET;
#endif
      writedata(TFT_MAD_MX | TFT_MAD_MV | TFT_MAD_COLOR_ORDER);

      _width  = _init_height;
      _height = _init_width;
      break;

    case 2: // Inverter portrait
#ifdef CGRAM_OFFSET
      if (_init_width == 135)
      {
        colstart = 53;
        rowstart = 40;
      }
      else if(_init_height == 280)
      {
        colstart = 0;
        rowstart = 20;
      }
      else
      {
        colstart = 0;
        rowstart = 0;
      }
#endif
#ifdef TFT_COLUMN_OFFSET
  colstart += TFT_COLUMN_OFFSET;
#endif
      writedata(TFT_MAD_MX | TFT_MAD_MY | TFT_MAD_COLOR_ORDER);

      _width  = _init_width;
      _height = _init_height;
       break;
    case 3: // Inverted landscape
#ifdef CGRAM_OFFSET
      if (_init_width == 135)
      {
        colstart = 40;
        rowstart = 52;
      }
      else if(_init_height == 280)
      {
        colstart = 20;
        rowstart = 0;
      }
      else
      {
        colstart = 0;
        rowstart = 0;
      }
#endif
#ifdef TFT_COLUMN_OFFSET
  colstart += TFT_COLUMN_OFFSET;
#endif
      writedata(TFT_MAD_MV | TFT_MAD_MY | TFT_MAD_COLOR_ORDER);

      _width  = _init_height;
      _height = _init_width;
      break;
  }
