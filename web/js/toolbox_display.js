
Blockly.Blocks['display_clearscreen'] = {
  init: function () {
      this.appendDummyInput()
          .appendField("全屏填充")
          .appendField(new Blockly.FieldDropdown([["白色", "white"], ["黑色", "black"]]), "color");
      this.setPreviousStatement(true, null);
      this.setNextStatement(true, null);
      this.setColour(230);
      this.setTooltip("");
      this.setHelpUrl("");
  }
};

Blockly.Blocks['display_display'] = {
  init: function () {
      this.appendDummyInput()
          .appendField("进行")
          .appendField(new Blockly.FieldDropdown([["全局", "full"], ["局部", "partial"]]), "refreshtype")
          .appendField("刷新");
      this.setPreviousStatement(true, null);
      this.setNextStatement(true, null);
      this.setColour(230);
      this.setTooltip("");
      this.setHelpUrl("");
  }
};

Blockly.Blocks['display_displaywindow'] = {
  init: function () {
      this.appendValueInput("pos_x")
          .setCheck("Number")
          .appendField("局部刷新以x=");
      this.appendValueInput("pos_y")
          .setCheck("Number")
          .appendField("y=");
      this.appendValueInput("pos_w")
          .setCheck("Number")
          .appendField("为左上角，宽");
      this.appendValueInput("pos_h")
          .setCheck("Number")
          .appendField("高");
      this.appendDummyInput()
          .appendField("的矩形区域");
      this.setInputsInline(true);
      this.setPreviousStatement(true, null);
      this.setNextStatement(true, null);
      this.setColour(230);
      this.setTooltip("");
      this.setHelpUrl("");
  }
};

Blockly.Blocks['display_drawline'] = {
  init: function () {
      this.appendValueInput("pos_x1")
          .setCheck("Number")
          .appendField("从x=");
      this.appendValueInput("pos_y1")
          .setCheck("Number")
          .appendField("y=");
      this.appendValueInput("pos_x2")
          .setCheck("Number")
          .appendField("到x=");
      this.appendValueInput("pos_y2")
          .setCheck("Number")
          .appendField("y=");
      this.appendDummyInput()
          .appendField("画一条")
          .appendField(new Blockly.FieldDropdown([["黑色", "black"], ["白色", "white"]]), "color")
          .appendField("的线");
      this.setInputsInline(true);
      this.setPreviousStatement(true, null);
      this.setNextStatement(true, null);
      this.setColour(230);
      this.setTooltip("");
      this.setHelpUrl("");
  }
};

Blockly.Blocks['display_setpartialwindow'] = {
  init: function () {
      this.appendValueInput("pos_x")
          .setCheck("Number")
          .appendField("把以x=");
      this.appendValueInput("pos_y")
          .setCheck("Number")
          .appendField("y=");
      this.appendValueInput("pos_w")
          .setCheck("Number")
          .appendField("为左上角，宽");
      this.appendValueInput("pos_h")
          .setCheck("Number")
          .appendField("高");
      this.appendDummyInput()
          .appendField("的矩形区域作为局部刷新窗口");
      this.setInputsInline(true);
      this.setPreviousStatement(true, null);
      this.setNextStatement(true, null);
      this.setColour(230);
      this.setTooltip("");
      this.setHelpUrl("");
  }
};

Blockly.Blocks['display_setfullwindow'] = {
  init: function () {
      this.appendDummyInput()
          .appendField("将刷新窗口设置为全屏");
      this.setPreviousStatement(true, null);
      this.setNextStatement(true, null);
      this.setColour(230);
      this.setTooltip("");
      this.setHelpUrl("");
  }
};

Blockly.Blocks['display_drawpixel'] = {
  init: function () {
      this.appendValueInput("pos_x1")
          .setCheck("Number")
          .appendField("在x=");
      this.appendValueInput("pos_y1")
          .setCheck("Number")
          .appendField("y=");
      this.appendDummyInput()
          .appendField("点一个")
          .appendField(new Blockly.FieldDropdown([["黑色", "black"], ["白色", "white"]]), "color")
          .appendField("的点");
      this.setInputsInline(true);
      this.setPreviousStatement(true, null);
      this.setNextStatement(true, null);
      this.setColour(230);
      this.setTooltip("");
      this.setHelpUrl("");
  }
};

Blockly.Blocks['display_fillrect'] = {
  init: function () {
      this.appendValueInput("pos_x")
          .setCheck("Number")
          .appendField("以x=");
      this.appendValueInput("pos_y")
          .setCheck("Number")
          .appendField("y=");
      this.appendValueInput("pos_w")
          .setCheck("Number")
          .appendField("为左上角，画一个宽");
      this.appendValueInput("pos_h")
          .setCheck("Number")
          .appendField("高");
      this.appendDummyInput()
          .appendField("的")
          .appendField(new Blockly.FieldDropdown([["黑色", "black"], ["白色", "white"]]), "color")
          .appendField("实心矩形");
      this.setInputsInline(true);
      this.setPreviousStatement(true, null);
      this.setNextStatement(true, null);
      this.setColour(230);
      this.setTooltip("");
      this.setHelpUrl("");
  }
};

Blockly.Blocks['display_drawrect'] = {
  init: function () {
      this.appendValueInput("pos_x")
          .setCheck("Number")
          .appendField("以x=");
      this.appendValueInput("pos_y")
          .setCheck("Number")
          .appendField("y=");
      this.appendValueInput("pos_w")
          .setCheck("Number")
          .appendField("为左上角，画一个宽");
      this.appendValueInput("pos_h")
          .setCheck("Number")
          .appendField("高");
      this.appendDummyInput()
          .appendField("的")
          .appendField(new Blockly.FieldDropdown([["黑色", "black"], ["白色", "white"]]), "color")
          .appendField("矩形");
      this.setInputsInline(true);
      this.setPreviousStatement(true, null);
      this.setNextStatement(true, null);
      this.setColour(230);
      this.setTooltip("");
      this.setHelpUrl("");
  }
};

Blockly.Blocks['display_drawroundrect'] = {
  init: function () {
      this.appendValueInput("pos_x")
          .setCheck("Number")
          .appendField("以x=");
      this.appendValueInput("pos_y")
          .setCheck("Number")
          .appendField("y=");
      this.appendValueInput("pos_w")
          .setCheck("Number")
          .appendField("为左上角，画一个宽");
      this.appendValueInput("pos_h")
          .setCheck("Number")
          .appendField("高");
      this.appendValueInput("radius")
          .setCheck("Number")
          .appendField("的，圆角半径为");
      this.appendDummyInput()
          .appendField("的")
          .appendField(new Blockly.FieldDropdown([["黑色", "black"], ["白色", "white"]]), "color")
          .appendField("圆角矩形");
      this.setInputsInline(true);
      this.setPreviousStatement(true, null);
      this.setNextStatement(true, null);
      this.setColour(230);
      this.setTooltip("");
      this.setHelpUrl("");
  }
};

Blockly.Blocks['display_fillroundrect'] = {
  init: function () {
      this.appendValueInput("pos_x")
          .setCheck("Number")
          .appendField("以x=");
      this.appendValueInput("pos_y")
          .setCheck("Number")
          .appendField("y=");
      this.appendValueInput("pos_w")
          .setCheck("Number")
          .appendField("为左上角，画一个宽");
      this.appendValueInput("pos_h")
          .setCheck("Number")
          .appendField("高");
      this.appendValueInput("radius")
          .setCheck("Number")
          .appendField("的，圆角半径为");
      this.appendDummyInput()
          .appendField("的")
          .appendField(new Blockly.FieldDropdown([["黑色", "black"], ["白色", "white"]]), "color")
          .appendField("实心圆角矩形");
      this.setInputsInline(true);
      this.setPreviousStatement(true, null);
      this.setNextStatement(true, null);
      this.setColour(230);
      this.setTooltip("");
      this.setHelpUrl("");
  }
};

Blockly.Blocks['display_drawcircle'] = {
  init: function () {
      this.appendValueInput("pos_x")
          .setCheck("Number")
          .appendField("以x=");
      this.appendValueInput("pos_y")
          .setCheck("Number")
          .appendField("y=");
      this.appendValueInput("radius")
          .setCheck("Number")
          .appendField("为圆心,");
      this.appendDummyInput()
          .appendField("为半径画一个")
          .appendField(new Blockly.FieldDropdown([["黑色", "black"], ["白色", "white"]]), "color")
          .appendField("的圆");
      this.setInputsInline(true);
      this.setPreviousStatement(true, null);
      this.setNextStatement(true, null);
      this.setColour(230);
      this.setTooltip("");
      this.setHelpUrl("");
  }
};

Blockly.Blocks['display_fillcircle'] = {
  init: function () {
      this.appendValueInput("pos_x")
          .setCheck("Number")
          .appendField("以x=");
      this.appendValueInput("pos_y")
          .setCheck("Number")
          .appendField("y=");
      this.appendValueInput("radius")
          .setCheck("Number")
          .appendField("为圆心,");
      this.appendDummyInput()
          .appendField("为半径画一个")
          .appendField(new Blockly.FieldDropdown([["黑色", "black"], ["白色", "white"]]), "color")
          .appendField("的实心圆");
      this.setInputsInline(true);
      this.setPreviousStatement(true, null);
      this.setNextStatement(true, null);
      this.setColour(230);
      this.setTooltip("");
      this.setHelpUrl("");
  }
};

Blockly.Blocks['display_setcursor'] = {
  init: function () {
      this.appendValueInput("pos_x")
          .setCheck("Number")
          .appendField("设置文字坐标为x=");
      this.appendValueInput("pos_y")
          .setCheck("Number")
          .appendField("y=");
      this.setInputsInline(true);
      this.setPreviousStatement(true, null);
      this.setNextStatement(true, null);
      this.setColour(230);
      this.setTooltip("");
      this.setHelpUrl("");
  }
};

Blockly.Blocks['display_setfont'] = {
  init: function () {
      this.appendDummyInput()
          .appendField("设置字体为")
          .appendField(new Blockly.FieldDropdown([["FreeSans9pt7b", "0"], ["FreeSans18pt7b", "1"], ["Picopixel", "2"]]), "font");
      this.setInputsInline(true);
      this.setPreviousStatement(true, null);
      this.setNextStatement(true, null);
      this.setColour(230);
      this.setTooltip("无法修改中文字体");
      this.setHelpUrl("");
  }
};

Blockly.Blocks['display_settextcolor'] = {
  init: function () {
      this.appendDummyInput()
          .appendField("设置文本前景色为")
          .appendField(new Blockly.FieldDropdown([["黑色", "black"], ["白色", "white"]]), "color");
      this.setInputsInline(true);
      this.setPreviousStatement(true, null);
      this.setNextStatement(true, null);
      this.setColour(230);
      this.setTooltip("");
      this.setHelpUrl("");
  }
};

Blockly.Blocks['display_setbackgroundcolor'] = {
  init: function () {
      this.appendDummyInput()
          .appendField("设置文本背景色为")
          .appendField(new Blockly.FieldDropdown([["白色", "white"], ["黑色", "black"]]), "color");
      this.setInputsInline(true);
      this.setPreviousStatement(true, null);
      this.setNextStatement(true, null);
      this.setColour(230);
      this.setTooltip("");
      this.setHelpUrl("");
  }
};

Blockly.Blocks['display_print'] = {
  init: function () {
      this.appendValueInput("str")
          .setCheck("String")
          .appendField("显示英文");
      this.setInputsInline(true);
      this.setPreviousStatement(true, null);
      this.setNextStatement(true, null);
      this.setColour(230);
      this.setTooltip("");
      this.setHelpUrl("");
  }
};

Blockly.Blocks['display_getcursorx'] = {
  init: function () {
      this.appendDummyInput()
          .appendField("当前x坐标");
      this.setInputsInline(true);
      this.setOutput(true, null);
      this.setColour(230);
      this.setTooltip("");
      this.setHelpUrl("");
  }
};

Blockly.Blocks['display_getcursory'] = {
  init: function () {
      this.appendDummyInput()
          .appendField("当前y坐标");
      this.setInputsInline(true);
      this.setOutput(true, null);
      this.setColour(230);
      this.setTooltip("");
      this.setHelpUrl("");
  }
};

Blockly.Blocks['display_u8g2getcursorx'] = {
  init: function () {
      this.appendDummyInput()
          .appendField("当前中文x坐标");
      this.setInputsInline(true);
      this.setOutput(true, null);
      this.setColour(230);
      this.setTooltip("");
      this.setHelpUrl("");
  }
};

Blockly.Blocks['display_u8g2getcursory'] = {
  init: function () {
      this.appendDummyInput()
          .appendField("当前中文y坐标");
      this.setInputsInline(true);
      this.setOutput(true, null);
      this.setColour(230);
      this.setTooltip("");
      this.setHelpUrl("");
  }
};

Blockly.Blocks['display_u8g2print'] = {
  init: function () {
      this.appendValueInput("str")
          .setCheck("String")
          .appendField("显示UTF-8字符");
      this.setInputsInline(true);
      this.setPreviousStatement(true, null);
      this.setNextStatement(true, null);
      this.setColour(230);
      this.setTooltip("");
      this.setHelpUrl("");
  }
};

Blockly.Blocks['display_drawlbm'] = {
  init: function () {
      this.appendValueInput("pos_x1")
          .setCheck("Number")
          .appendField("在x=");
      this.appendValueInput("pos_y1")
          .setCheck("Number")
          .appendField("y=");
      this.appendDummyInput()
          .appendField("处绘制前景为")
          .appendField(new Blockly.FieldDropdown([["黑色", "black"], ["白色", "white"]]), "color");
      this.appendValueInput("file")
          .setCheck("String")
          .appendField("的LBM图像文件：");
      this.setInputsInline(true);
      this.setPreviousStatement(true, null);
      this.setNextStatement(true, null);
      this.setColour(230);
      this.setTooltip("背景透明");
      this.setHelpUrl("");
  }
};

function colorToNum(color) {
  if (color == 'white') {
    return '1';
  } else {
    return '0';
  }
}
lua.luaGenerator.forBlock['display_clearscreen'] = function (block, generator) {
  var dropdown_color = block.getFieldValue('color');
  var code = 'display.clearScreen(' + colorToNum(dropdown_color) + ')\n';
  return code;
};

lua.luaGenerator.forBlock['display_display'] = function (block, generator) {
  var dropdown_refreshtype = block.getFieldValue('refreshtype');
  if (dropdown_refreshtype == 'partial') {
    dropdown_refreshtype = 'true';
  }
  else {
    dropdown_refreshtype = 'false';
  }
  var code = 'display.display(' + dropdown_refreshtype + ')\n';
  return code;
};

lua.luaGenerator.forBlock['display_displaywindow'] = function (block, generator) {
  var value_pos_x = generator.valueToCode(block, 'pos_x', lua.Order.ATOMIC);
  var value_pos_y = generator.valueToCode(block, 'pos_y', lua.Order.ATOMIC);
  var value_pos_w = generator.valueToCode(block, 'pos_w', lua.Order.ATOMIC);
  var value_pos_h = generator.valueToCode(block, 'pos_h', lua.Order.ATOMIC);
  var code = 'display.displayWindow(' + value_pos_x + ', ' + value_pos_y + ', ' + value_pos_w + ', ' + value_pos_h + ')\n';
  return code;
};

lua.luaGenerator.forBlock['display_drawline'] = function (block, generator) {
  var value_pos_x1 = generator.valueToCode(block, 'pos_x1', lua.Order.ATOMIC);
  var value_pos_y1 = generator.valueToCode(block, 'pos_y1', lua.Order.ATOMIC);
  var value_pos_x2 = generator.valueToCode(block, 'pos_x2', lua.Order.ATOMIC);
  var value_pos_y2 = generator.valueToCode(block, 'pos_y2', lua.Order.ATOMIC);
  var dropdown_color = colorToNum(block.getFieldValue('color'));
  var code = 'display.drawLine(' + value_pos_x1 + ', ' + value_pos_y1 + ', ' + value_pos_x2 + ', ' + value_pos_y2 + ', ' + dropdown_color + ')\n';
  return code;
};

lua.luaGenerator.forBlock['display_setpartialwindow'] = function (block, generator) {
  var value_pos_x = generator.valueToCode(block, 'pos_x', lua.Order.ATOMIC);
  var value_pos_y = generator.valueToCode(block, 'pos_y', lua.Order.ATOMIC);
  var value_pos_w = generator.valueToCode(block, 'pos_w', lua.Order.ATOMIC);
  var value_pos_h = generator.valueToCode(block, 'pos_h', lua.Order.ATOMIC);
  var code = 'display.setPartialWindow(' + value_pos_x + ', ' + value_pos_y + ', ' + value_pos_w + ', ' + value_pos_h + ')\n';
  return code;
};

lua.luaGenerator.forBlock['display_setfullwindow'] = function (block, generator) {
  var code = 'display.setFullWindow()\n';
  return code;
};

lua.luaGenerator.forBlock['display_drawpixel'] = function (block, generator) {
  var value_pos_x1 = generator.valueToCode(block, 'pos_x1', lua.Order.ATOMIC);
  var value_pos_y1 = generator.valueToCode(block, 'pos_y1', lua.Order.ATOMIC);
  var dropdown_color = colorToNum(block.getFieldValue('color'));
  var code = 'display.drawPixel(' + value_pos_x1 + ', ' + value_pos_y1 + ', ' + dropdown_color + ')\n';
  return code;
};

lua.luaGenerator.forBlock['display_fillrect'] = function (block, generator) {
  var value_pos_x = generator.valueToCode(block, 'pos_x', lua.Order.ATOMIC);
  var value_pos_y = generator.valueToCode(block, 'pos_y', lua.Order.ATOMIC);
  var value_pos_w = generator.valueToCode(block, 'pos_w', lua.Order.ATOMIC);
  var value_pos_h = generator.valueToCode(block, 'pos_h', lua.Order.ATOMIC);
  var dropdown_color = colorToNum(block.getFieldValue('color'));
  var code = 'display.fillRect(' + value_pos_x + ', ' + value_pos_y + ', ' + value_pos_w + ', ' + value_pos_h + ', ' + dropdown_color + ')\n';
  return code;
};

lua.luaGenerator.forBlock['display_drawrect'] = function (block, generator) {
  var value_pos_x = generator.valueToCode(block, 'pos_x', lua.Order.ATOMIC);
  var value_pos_y = generator.valueToCode(block, 'pos_y', lua.Order.ATOMIC);
  var value_pos_w = generator.valueToCode(block, 'pos_w', lua.Order.ATOMIC);
  var value_pos_h = generator.valueToCode(block, 'pos_h', lua.Order.ATOMIC);
  var dropdown_color = colorToNum(block.getFieldValue('color'));
  var code = 'display.drawRect(' + value_pos_x + ', ' + value_pos_y + ', ' + value_pos_w + ', ' + value_pos_h + ', ' + dropdown_color + ')\n';
  return code;
};

lua.luaGenerator.forBlock['display_drawroundrect'] = function (block, generator) {
  var value_pos_x = generator.valueToCode(block, 'pos_x', lua.Order.ATOMIC);
  var value_pos_y = generator.valueToCode(block, 'pos_y', lua.Order.ATOMIC);
  var value_pos_w = generator.valueToCode(block, 'pos_w', lua.Order.ATOMIC);
  var value_pos_h = generator.valueToCode(block, 'pos_h', lua.Order.ATOMIC);
  var value_radius = generator.valueToCode(block, 'radius', lua.Order.ATOMIC);
  var dropdown_color = colorToNum(block.getFieldValue('color'));
  var code = 'display.drawRoundRect(' + value_pos_x + ', ' + value_pos_y + ', ' + value_pos_w + ', ' + value_pos_h + ', ' + value_radius + ', ' + dropdown_color + ')\n';
  return code;
};

lua.luaGenerator.forBlock['display_fillroundrect'] = function (block, generator) {
  var value_pos_x = generator.valueToCode(block, 'pos_x', lua.Order.ATOMIC);
  var value_pos_y = generator.valueToCode(block, 'pos_y', lua.Order.ATOMIC);
  var value_pos_w = generator.valueToCode(block, 'pos_w', lua.Order.ATOMIC);
  var value_pos_h = generator.valueToCode(block, 'pos_h', lua.Order.ATOMIC);
  var value_radius = generator.valueToCode(block, 'radius', lua.Order.ATOMIC);
  var dropdown_color = colorToNum(block.getFieldValue('color'));
  var code = 'display.fillRoundRect(' + value_pos_x + ', ' + value_pos_y + ', ' + value_pos_w + ', ' + value_pos_h + ', ' + value_radius + ', ' + dropdown_color + ')\n';
  return code;
};

lua.luaGenerator.forBlock['display_drawcircle'] = function (block, generator) {
  var value_pos_x = generator.valueToCode(block, 'pos_x', lua.Order.ATOMIC);
  var value_pos_y = generator.valueToCode(block, 'pos_y', lua.Order.ATOMIC);
  var value_radius = generator.valueToCode(block, 'radius', lua.Order.ATOMIC);
  var dropdown_color = colorToNum(block.getFieldValue('color'));
  var code = 'display.drawCircle(' + value_pos_x + ', ' + value_pos_y + ', ' + value_radius + ', ' + dropdown_color + ')\n';
  return code;
};

lua.luaGenerator.forBlock['display_fillcircle'] = function (block, generator) {
  var value_pos_x = generator.valueToCode(block, 'pos_x', lua.Order.ATOMIC);
  var value_pos_y = generator.valueToCode(block, 'pos_y', lua.Order.ATOMIC);
  var value_radius = generator.valueToCode(block, 'radius', lua.Order.ATOMIC);
  var dropdown_color = colorToNum(block.getFieldValue('color'));
  var code = 'display.fillCircle(' + value_pos_x + ', ' + value_pos_y + ', ' + value_radius + ', ' + dropdown_color + ')\n';
  return code;
};

lua.luaGenerator.forBlock['display_setcursor'] = function (block, generator) {
  var value_pos_x = generator.valueToCode(block, 'pos_x', lua.Order.ATOMIC);
  var value_pos_y = generator.valueToCode(block, 'pos_y', lua.Order.ATOMIC);
  var code = 'display.setCursor(' + value_pos_x + ', ' + value_pos_y + ')\n';
  return code;
};

lua.luaGenerator.forBlock['display_setfont'] = function (block, generator) {
  var dropdown_font = block.getFieldValue('font');
  var code = 'display.setFont(' + dropdown_font + ')\n';
  return code;
};

lua.luaGenerator.forBlock['display_settextcolor'] = function (block, generator) {
  var dropdown_color = colorToNum(block.getFieldValue('color'));
  var code = 'display.setTextColor(' + dropdown_color + ')\n';
  return code;
};

lua.luaGenerator.forBlock['display_setbackgroundcolor'] = function (block, generator) {
  var dropdown_color = block.getFieldValue('color');
  var code = 'display.setBackgroundColor(' + dropdown_color + ')\n';
  return code;
};

lua.luaGenerator.forBlock['display_print'] = function (block, generator) {
  var value_str = generator.valueToCode(block, 'str', lua.Order.ATOMIC);
  var code = 'display.print(' + value_str + ')\n';
  return code;
};

lua.luaGenerator.forBlock['display_getcursorx'] = function (block, generator) {
  var code = 'display.getCursorX()';
  return [code, lua.ATOMIC];
};

lua.luaGenerator.forBlock['display_getcursory'] = function (block, generator) {
  var code = 'display.getCursorY()';
  return [code, lua.ATOMIC];
};

lua.luaGenerator.forBlock['display_u8g2getcursorx'] = function (block, generator) {
  var code = 'display.u8g2GetCursorX()';
  return [code, lua.ATOMIC];
};

lua.luaGenerator.forBlock['display_u8g2getcursory'] = function (block, generator) {
  var code = 'display.u8g2GetCursorY()';
  return [code, lua.ATOMIC];
};

lua.luaGenerator.forBlock['display_u8g2print'] = function (block, generator) {
  var value_str = generator.valueToCode(block, 'str', lua.Order.ATOMIC);
  var code = 'display.u8g2Print(' + value_str + ')\n';
  return code;
};

lua.luaGenerator.forBlock['display_drawlbm'] = function (block, generator) {
  var value_pos_x1 = generator.valueToCode(block, 'pos_x1', lua.Order.ATOMIC);
  var value_pos_y1 = generator.valueToCode(block, 'pos_y1', lua.Order.ATOMIC);
  var value_file = generator.valueToCode(block, 'file', lua.Order.ATOMIC);
  var dropdown_color = colorToNum(block.getFieldValue('color'));
  var code = 'display.u8g2Print(' + value_pos_x1 + ', ' + value_pos_y1 + ', ' + value_file + ', ' + dropdown_color + ')\n';
  return code;
};