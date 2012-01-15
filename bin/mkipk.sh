SHARP_LIB_DIR=/opt/Qtopia/sharp/lib

mv "$SHARP_LIB_DIR/libz.so" "$SHARP_LIB_DIR/libz.so.disabled"
mv "$SHARP_LIB_DIR/libz.so.1" "$SHARP_LIB_DIR/libz.so.1.disabled"
mv "$SHARP_LIB_DIR/libz.so.1.2.5" "$SHARP_LIB_DIR/libz.so.1.2.5.disabled"

mkipks toMOTko.control

mv "$SHARP_LIB_DIR/libz.so.1.2.5.disabled" "$SHARP_LIB_DIR/libz.so.1.2.5"
mv "$SHARP_LIB_DIR/libz.so.1.disabled" "$SHARP_LIB_DIR/libz.so.1"
mv "$SHARP_LIB_DIR/libz.so.disabled" "$SHARP_LIB_DIR/libz.so"
