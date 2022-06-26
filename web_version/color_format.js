function RGBA_to_LAB(color) {
  var rgb = color.toRGBA();
  var var_R = rgb[0] / 255;
  var var_G = rgb[1] / 255;
  var var_B = rgb[2] / 255;
  if (var_R > 0.04045)
    var_R = Math.pow(((var_R + 0.055) / 1.055), 2.4);
  else
    var_R = var_R / 12.92;
  if (var_G > 0.04045)
    var_G = Math.pow(((var_G + 0.055) / 1.055), 2.4);
  else
    var_G = var_G / 12.92;
  if (var_B > 0.04045)
    var_B = Math.pow(((var_B + 0.055) / 1.055), 2.4);
  else
    var_B = var_B / 12.92;

  var_R = var_R * 100;
  var_G = var_G * 100;
  var_B = var_B * 100;

  var X = var_R * 0.4124 + var_G * 0.3576 + var_B * 0.1805;
  var Y = var_R * 0.2126 + var_G * 0.7152 + var_B * 0.0722;
  var Z = var_R * 0.0193 + var_G * 0.1192 + var_B * 0.9505;

  var var_X = X / 95.047;
  var var_Y = Y / 100.0;
  var var_Z = Z / 108.883;

  if (var_X > 0.008856)
    var_X = Math.pow(var_X, (1.0 / 3.0));
  else
    var_X = (7.787 * var_X) + (16.0 / 116.0);
  if (var_Y > 0.008856)
    var_Y = Math.pow(var_Y, (1.0 / 3.0));
  else
    var_Y = (7.787 * var_Y) + (16.0 / 116.0);
  if (var_Z > 0.008856)
    var_Z = Math.pow(var_Z, (1.0 / 3.0));
  else
    var_Z = (7.787 * var_Z) + (16.0 / 116.0);

  const CIE_L = (116.0 * var_Y) - 16.0;
  const CIE_a = 500.0 * (var_X - var_Y);
  const CIE_b = 200.0 * (var_Y - var_Z);
  return [CIE_L, CIE_a, CIE_b];
}

function LAB_Distance(lab1, lab2){
    return (
      Math.pow(lab1[0] - lab2[0], 2) +
      Math.pow(lab1[1] - lab2[1], 2) +
      Math.pow(lab1[2] - lab2[2], 2)) / 100.0;
}

class ColorPoint {
  constructor(elm, default_color) {
    this.pickr = new Pickr({
      el: elm,
      default: default_color,
      theme: 'classic',
      lockOpacity: true,
      components: {
        preview: false,
        opacity: false,
        hue: true,
        interaction: {
          hex: true,
          rgba: true,
          hsva: true,
          input: true,
          clear: true,
          save: false
        }
      }
    });
    this.pickr.on('change', (color, source, instance) => {
      this.LAB = RGBA_to_LAB(color);
      this.pickr.applyColor();
    }).on('init', instance => {
      this.LAB = RGBA_to_LAB(this.pickr._color);
    });
    this.LAB = RGBA_to_LAB(this.pickr._color);
    // console.log(this.pickr);
  }

  remove_element() {
    const root = this.pickr.getRoot().root;
    root.parentElement.removeChild(root);
  }

  get_color(){
    return this.pickr._color.toHEXA().toString();
  }

  set_color(input){
    this.pickr.setColor(input);
  }
}