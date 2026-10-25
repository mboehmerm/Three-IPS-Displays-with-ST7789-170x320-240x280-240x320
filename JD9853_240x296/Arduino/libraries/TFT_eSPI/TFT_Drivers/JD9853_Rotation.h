
// This is the command sequence that rotates the JD9853 driver coordinate frame

  rotation = m % 8;

  writecommand(TFT_MADCTL);
  switch (rotation) {
    case 0:
      writedata(0x00);
      _width  = _init_width;
      _height = _init_height;
#ifdef CGRAM_OFFSET
      if (_init_height == 296)
      {
        colstart = 0;
        rowstart = 0;
      }
#endif
      break;
    case 1:
      writedata(TFT_MAD_MV | TFT_MAD_MX);
      _width  = _init_height;
      _height = _init_width;
#ifdef CGRAM_OFFSET
      if (_init_height == 296)
      {
        colstart = 0;
        rowstart = 0;
      }
#endif
      break;
    case 2:
      writedata(TFT_MAD_MX | TFT_MAD_MY);
      _width  = _init_width;
      _height = _init_height;
#ifdef CGRAM_OFFSET
      if (_init_height == 296)
      {
        colstart = 0;
        rowstart = 24;
      }
#endif    
      break;
    case 3:
      writedata(TFT_MAD_MV | TFT_MAD_MY);
      _width  = _init_height;
      _height = _init_width;
#ifdef CGRAM_OFFSET
      if (_init_height == 296)
      {
        colstart = 24;
        rowstart = 0;
      }
#endif
      break;

    case 4:
      writedata(TFT_MAD_MX);
      _width  = _init_width;
      _height = _init_height;
#ifdef CGRAM_OFFSET
      if (_init_height == 296)
      {
        colstart = 0;
        rowstart = 0;
      }
#endif    
      break;
    case 5:
      writedata(TFT_MAD_MV | TFT_MAD_MY | TFT_MAD_MX);
      _width  = _init_height;
      _height = _init_width;
#ifdef CGRAM_OFFSET
      if (_init_height == 296)
      {
        colstart = 24;
        rowstart = 0;
      }
#endif
      break;
    case 6:
      writedata(TFT_MAD_MY);
      _width  = _init_width;
      _height = _init_height;
#ifdef CGRAM_OFFSET
      if (_init_height == 296)
      {
        colstart = 0;
        rowstart = 24;
      }
#endif
      break;
    case 7:
      writedata(TFT_MAD_MV | TFT_MAD_ML | TFT_MAD_MH);
      _width  = _init_height;
      _height = _init_width;
#ifdef CGRAM_OFFSET
      if (_init_height == 296)
      {
        colstart = 24;
        rowstart = 0;
      }
#endif
      break;

  }
