
//  フォントデータ (8W x 16H pixel)　
//   1文字 = 16 byte = ( 8/8 *16 )
//
//  index  表示文字  
//    0       0   
//    1       1  
//    2       2
//    3       3
//    4       4
//    5       5
//    6       6
//    7       7
//    8       8
//    9       9 
//


const uint8_t font_8w_16h_seg[10][16] = {

{			// 0
0x00,
0x00,
0x00,
0x1c,
0x22,
0x41,
0x41,
0x41,
0x41,
0x41,
0x41,
0x41,
0x22,
0x1c,
0x00,
0x00
},


{			// 1
0x00,
0x00,
0x08,
0x18,
0x28,
0x48,
0x08,
0x08,
0x08,
0x08,
0x08,
0x08,
0x08,
0x08,
0x00,
0x00
},


{			// 2
0x00,
0x00,
0x00,
0x3e,
0x41,
0x41,
0x01,
0x02,
0x04,
0x08,
0x10,
0x20,
0x40,
0x7f,
0x00,
0x00

},

{			// 3
0x00,
0x00,
0x00,
0x3c,
0x42,
0x02,
0x02,
0x0c,
0x0c,
0x02,
0x02,
0x02,
0x42,
0x3c,
0x00,
0x00

},

{			// 4
0x00,
0x00,
0x00,
0x04,
0x0c,
0x14,
0x14,
0x24,
0x44,
0x44,
0x7f,
0x04,
0x04,
0x04,
0x00,
0x00

},


{			// 5
0x00,
0x00,
0x00,
0x3e,
0x20,
0x20,
0x20,
0x3e,
0x01,
0x01,
0x01,
0x01,
0x21,
0x1e,
0x00,
0x00

},


{			// 6
0x00,
0x00,
0x00,
0x3c,
0x42,
0x40,
0x40,
0x5c,
0x62,
0x41,
0x41,
0x41,
0x22,
0x1c,
0x00,
0x00
},

{			// 7
0x00,
0x00,
0x00,
0x7f,
0x02,
0x04,
0x04,
0x08,
0x08,
0x10,
0x10,
0x10,
0x10,
0x10,
0x00,
0x00

},

{			// 8
0x00,
0x00,
0x1c,
0x22,
0x41,
0x41,
0x22,
0x1c,
0x22,
0x41,
0x41,
0x41,
0x22,
0x1c,
0x00,
0x00

},

{			// 9
0x00,
0x00,
0x00,
0x1c,
0x22,
0x41,
0x41,
0x41,
0x21,
0x1f,
0x01,
0x01,
0x02,
0x3c,
0x00,
0x00

},


};