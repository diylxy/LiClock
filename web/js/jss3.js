
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
  var dropdown_color = colorToNum(block.getFieldValue('color'));
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
  return [code, lua.Order.ATOMIC];
};

lua.luaGenerator.forBlock['display_getcursory'] = function (block, generator) {
  var code = 'display.getCursorY()';
  return [code, lua.Order.ATOMIC];
};

lua.luaGenerator.forBlock['display_u8g2getcursorx'] = function (block, generator) {
  var code = 'display.u8g2GetCursorX()';
  return [code, lua.Order.ATOMIC];
};

lua.luaGenerator.forBlock['display_u8g2getcursory'] = function (block, generator) {
  var code = 'display.u8g2GetCursorY()';
  return [code, lua.Order.ATOMIC];
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
  var code = 'gui.drawLBM(' + value_pos_x1 + ', ' + value_pos_y1 + ', ' + value_file + ', ' + dropdown_color + ')\n';
  return code;
};
// AppManager
Blockly.Blocks['appmanager_gotoapp'] = {
  init: function() {
    this.appendValueInput("appName")
        .setCheck("String")
        .appendField("跳转到App");
    this.setPreviousStatement(true, null);
    this.setNextStatement(true, null);
    this.setColour(315);
 this.setTooltip("不会立即结束此App，在调试时无效");
 this.setHelpUrl("");
  }
};

Blockly.Blocks['appmanager_goback'] = {
  init: function() {
    this.appendDummyInput()
        .appendField("返回上个App");
    this.setPreviousStatement(true, null);
    this.setColour(315);
 this.setTooltip("同时立即结束此App，在调试时无效");
 this.setHelpUrl("");
  }
};

Blockly.Blocks['appmanager_nextwakeup'] = {
  init: function() {
    this.appendValueInput("next")
        .setCheck("Number")
        .appendField("设置间隔");
    this.appendDummyInput()
        .appendField("秒后唤醒");
    this.setPreviousStatement(true, null);
    this.setNextStatement(true, null);
    this.setColour(315);
 this.setTooltip("设置为0则永不自动唤醒");
 this.setHelpUrl("");
  }
};
lua.luaGenerator.forBlock['appmanager_gotoapp'] = function(block, generator) {
  var value_appname = generator.valueToCode(block, 'appName', lua.Order.ATOMIC);
  var code = 'appManager.gotoApp(' + value_appname + ')\n';
  return code;
};

lua.luaGenerator.forBlock['appmanager_goback'] = function(block, generator) {
  var code = 'appManager.goBack()\nreturn\n';
  return code;
};

lua.luaGenerator.forBlock['appmanager_nextwakeup'] = function(block, generator) {
  var value_next = generator.valueToCode(block, 'next', lua.Order.ATOMIC);
  var code = 'appManager.nextWakeup(' + value_next + ')\n';
  return code;
};
//Buzzer
Blockly.Blocks['buzzer_append'] = {
  init: function() {
    this.appendValueInput("freq")
        .setCheck("Number")
        .appendField("使蜂鸣器发出");
    this.appendValueInput("duri")
        .setCheck("Number")
        .appendField("Hz的声音，持续");
    this.appendDummyInput()
        .appendField("毫秒");
    this.setInputsInline(true);
    this.setPreviousStatement(true, null);
    this.setNextStatement(true, null);
    this.setColour(165);
 this.setTooltip("异步");
 this.setHelpUrl("");
  }
};

Blockly.Blocks['buzzer_playfile'] = {
  init: function() {
    this.appendValueInput("filename")
        .setCheck("String")
        .appendField("播放buz文件：");
    this.setPreviousStatement(true, null);
    this.setNextStatement(true, null);
    this.setColour(165);
 this.setTooltip("异步");
 this.setHelpUrl("");
  }
};

Blockly.Blocks['buzzer_forcestop'] = {
  init: function() {
    this.appendDummyInput()
        .appendField("立即终止蜂鸣器播放");
    this.setPreviousStatement(true, null);
    this.setNextStatement(true, null);
    this.setColour(165);
 this.setTooltip("并清空播放队列");
 this.setHelpUrl("");
  }
};
lua.luaGenerator.forBlock['buzzer_append'] = function(block, generator) {
  var value_freq = generator.valueToCode(block, 'freq', lua.Order.ATOMIC);
  var value_duri = generator.valueToCode(block, 'duri', lua.Order.ATOMIC);
  var code = 'buzzer.append(' + value_freq + ', ' + value_duri + ')\n';
  return code;
};

lua.luaGenerator.forBlock['buzzer_playfile'] = function(block, generator) {
  var value_filename = generator.valueToCode(block, 'filename', lua.Order.ATOMIC);
  var code = 'buzzer.playFile(' + value_filename + ')\n';
  return code;
};

lua.luaGenerator.forBlock['buzzer_forcestop'] = function(block, generator) {
  var code = 'buzzer.forceStop()\n';
  return code;
};
//GUI
Blockly.Blocks['gui_waitlongpress'] = {
  init: function() {
    this.appendDummyInput()
        .appendField("正在长按")
        .appendField(new Blockly.FieldDropdown([["左键","35"], ["中键","34"], ["右键","39"]]), "btn");
    this.setOutput(true, null);
    this.setColour(0);
 this.setTooltip("");
 this.setHelpUrl("");
  }
};

Blockly.Blocks['gui_autoindentdraw'] = {
  init: function() {
    this.appendValueInput("from")
        .setCheck("Number")
        .appendField("在x=");
    this.appendValueInput("to")
        .setCheck("Number")
        .appendField("到");
    this.appendValueInput("str")
        .setCheck("String")
        .appendField("范围内自动换行地显示");
    this.setPreviousStatement(true, null);
    this.setNextStatement(true, null);
    this.setColour(0);
 this.setTooltip("");
 this.setHelpUrl("");
  }
};

Blockly.Blocks['gui_drawwindowswithtitle'] = {
  init: function() {
    this.appendDummyInput()
        .appendField("画一个窗口框架");
    this.appendValueInput("str")
        .setCheck("String")
        .appendField("标题为");
    this.appendValueInput("x")
        .setCheck("Number")
        .appendField("左上角x=");
    this.appendValueInput("y")
        .setCheck("Number")
        .appendField("左上角y=");
    this.appendValueInput("w")
        .setCheck("Number")
        .appendField("宽=");
    this.appendValueInput("h")
        .setCheck("Number")
        .appendField("高=");
    this.setInputsInline(false);
    this.setPreviousStatement(true, null);
    this.setNextStatement(true, null);
    this.setColour(0);
 this.setTooltip("");
 this.setHelpUrl("");
  }
};

Blockly.Blocks['gui_msgbox'] = {
  init: function() {
    this.appendValueInput("title")
        .setCheck("String")
        .appendField("弹出标题为");
    this.appendValueInput("content")
        .setCheck("String")
        .appendField("内容为");
    this.appendDummyInput()
        .appendField("的消息框");
    this.setInputsInline(false);
    this.setPreviousStatement(true, null);
    this.setNextStatement(true, null);
    this.setColour(0);
 this.setTooltip("");
 this.setHelpUrl("");
  }
};

Blockly.Blocks['gui_msgbox_yn'] = {
  init: function() {
    this.appendValueInput("title")
        .setCheck("String")
        .appendField("弹出标题为");
    this.appendValueInput("content")
        .setCheck("String")
        .appendField("内容为");
    this.appendDummyInput()
        .appendField("的选择框，");
    this.appendValueInput("yes")
        .setCheck("String")
        .appendField("确认按钮显示");
    this.appendValueInput("no")
        .setCheck("String")
        .appendField("取消按钮显示");
    this.setInputsInline(false);
    this.setOutput(true, "Boolean");
    this.setColour(0);
 this.setTooltip("");
 this.setHelpUrl("");
  }
};

Blockly.Blocks['gui_msgbox_number'] = {
  init: function() {
    this.appendValueInput("title")
        .setCheck("String")
        .appendField("以");
    this.appendValueInput("digits")
        .setCheck("Number")
        .appendField("为标题输入");
    this.appendValueInput("pre")
        .setCheck("Number")
        .appendField("位数，预输入");
    this.setInputsInline(true);
    this.setOutput(true, "Number");
    this.setColour(0);
 this.setTooltip("");
 this.setHelpUrl("");
  }
};

Blockly.Blocks['gui_menu'] = {
  init: function() {
    this.appendValueInput("title")
        .setCheck("String")
        .appendField("弹出标题为：");
    this.appendValueInput("list")
        .setCheck("Array")
        .appendField("的菜单");
    this.setInputsInline(false);
    this.setOutput(true, "Number");
    this.setColour(0);
 this.setTooltip("并返回选择的项目ID");
 this.setHelpUrl("");
  }
};

Blockly.Blocks['gui_filedialog'] = {
  init: function() {
    this.appendValueInput("title")
        .setCheck("String")
        .appendField("弹出标题为：");
    this.appendDummyInput()
        .appendField("的文件菜单");
    this.setInputsInline(false);
    this.setOutput(true, "String");
    this.setColour(0);
 this.setTooltip("");
 this.setHelpUrl("");
  }
};
lua.luaGenerator.forBlock['gui_waitlongpress'] = function(block, generator) {
  var dropdown_btn = block.getFieldValue('btn');
  // TODO: Assemble lua into code variable.
  var code = 'gui.waitLongpress(' + dropdown_btn + ')';
  return [code, lua.Order.ATOMIC];
};

lua.luaGenerator.forBlock['gui_autoindentdraw'] = function(block, generator) {
  var value_from = generator.valueToCode(block, 'from', lua.Order.ATOMIC);
  var value_to = generator.valueToCode(block, 'to', lua.Order.ATOMIC);
  var value_str = generator.valueToCode(block, 'str', lua.Order.ATOMIC);
  var code = 'gui.autoIndentDraw(' + value_str + ', ' + value_to + ', ' + value_from + ')\n';
  return code;
};

lua.luaGenerator.forBlock['gui_drawwindowswithtitle'] = function(block, generator) {
  var value_str = generator.valueToCode(block, 'str', lua.Order.ATOMIC);
  var value_x = generator.valueToCode(block, 'x', lua.Order.ATOMIC);
  var value_y = generator.valueToCode(block, 'y', lua.Order.ATOMIC);
  var value_w = generator.valueToCode(block, 'w', lua.Order.ATOMIC);
  var value_h = generator.valueToCode(block, 'h', lua.Order.ATOMIC);
  var code = 'gui.drawWindowsWithTitle(' + value_str + ', ' + value_x + ', ' + value_y + ', ' + value_w + ', ' + value_h + ')\n';
  return code;
};

lua.luaGenerator.forBlock['gui_msgbox'] = function(block, generator) {
  var value_title = generator.valueToCode(block, 'title', lua.Order.ATOMIC);
  var value_content = generator.valueToCode(block, 'content', lua.Order.ATOMIC);
  var code = 'gui.msgbox(' + value_title + ', ' + value_content + ')\n';
  return code;
};

lua.luaGenerator.forBlock['gui_msgbox_yn'] = function(block, generator) {
  var value_title = generator.valueToCode(block, 'title', lua.Order.ATOMIC);
  var value_content = generator.valueToCode(block, 'content', lua.Order.ATOMIC);
  var value_yes = generator.valueToCode(block, 'yes', lua.Order.ATOMIC);
  var value_no = generator.valueToCode(block, 'no', lua.Order.ATOMIC);
  var code = 'gui.msgbox_yn(' + value_title + ', ' + value_content + ', ' + value_yes + ', ' + value_no + ')';
  return [code, lua.Order.ATOMIC];
};

lua.luaGenerator.forBlock['gui_msgbox_number'] = function(block, generator) {
  var value_title = generator.valueToCode(block, 'title', lua.Order.ATOMIC);
  var value_digits = generator.valueToCode(block, 'digits', lua.Order.ATOMIC);
  var value_pre = generator.valueToCode(block, 'pre', lua.Order.ATOMIC);
  var code = 'gui.msgbox_number(' + value_title + ',' + value_digits + ', ' + value_pre + ')';
  return [code, lua.Order.ATOMIC];
};

lua.luaGenerator.forBlock['gui_menu'] = function(block, generator) {
  var value_title = generator.valueToCode(block, 'title', lua.Order.ATOMIC);
  var value_list = generator.valueToCode(block, 'list', lua.Order.ATOMIC);
  // TODO: Assemble lua into code variable.
  var code = 'gui.menu(' + value_title + ', ' + value_list + ')';
  // TODO: Change ORDER_NONE to the correct strength.
  return [code, lua.Order.ATOMIC];
};

lua.luaGenerator.forBlock['gui_filedialog'] = function(block, generator) {
  var value_title = generator.valueToCode(block, 'title', lua.Order.ATOMIC);
  var code = 'gui.fileDialog(' + value_title + ')';
  return [code, lua.Order.ATOMIC];
};