#include "conioex.h"
#include <iostream>
#include <conio.h>
#include <cstdio> // printfを使用するために追加
#include <cmath> // atan2, roundを使用するために追加
#include <algorithm> // std::max, std::minを使用するために追加
#include <string> // stringを使用するために追加
#include <vector> // vectorを使用するために追加

#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

// スティックのニュートラルポジションを定義
const int NEUTRAL_X = 32767;
const int NEUTRAL_Y = 32511;
const double MAX_STICK_VALUE = 32768.0; // スティックの最大値 (ニュートラルからの差)

// ボタンのビットマスク定義
enum ButtonMasks {
    BUTTON_Y = 1,
    BUTTON_B = 2,
    BUTTON_A = 4,
    BUTTON_X = 8,
    BUTTON_L_BUMPER = 16,    // 'l'
    BUTTON_R_BUMPER = 32,    // 'r'
    BUTTON_L_TRIGGER = 64,   // 'L'
    BUTTON_R_TRIGGER = 128,  // 'R'
    BUTTON_LS_PRESS = 256,   // 'ls'
    BUTTON_RS_PRESS = 512,  // 'rs'
    BUTTON_BACK = 1024,
    BUTTON_START = 2048,
    BUTTON_GUIDE = 4096
};

// スティックの傾きを描画する関数
void draw_stick_visual(int start_x, int start_y, double stick_x, double stick_y, const char* title) {
    const int VIZ_SIZE = 11; // 描画領域のサイズ (奇数)
    const int VIZ_CENTER = VIZ_SIZE / 2;

    // タイトルを描画
    gotoxy(start_x, start_y);
    printf(title);

    // 描画領域をクリアし、枠を描画
    for (int y = 0; y < VIZ_SIZE; ++y) {
        gotoxy(start_x, start_y + 1 + y);
        for (int x = 0; x < VIZ_SIZE; ++x) {
            if (y == 0 || y == VIZ_SIZE - 1 || x == 0 || x == VIZ_SIZE - 1) {
                putch('+'); // 枠
            }
            else if (y == VIZ_CENTER && x == VIZ_CENTER) {
                putch('.'); // 中心
            }
            else {
                putch(' '); // 空白
            }
        }
    }

    // スティックの値を描画領域の座標に変換
    // Y軸はコンソール座標系に合わせて反転させる
    int indicator_x = static_cast<int>(round(stick_x / MAX_STICK_VALUE * VIZ_CENTER)) + VIZ_CENTER;
    int indicator_y = static_cast<int>(round(stick_y / MAX_STICK_VALUE * VIZ_CENTER)) + VIZ_CENTER;

    // 描画領域内に収める (マクロとの競合を避けるため括弧で囲む)
    indicator_x = (std::max)(0, (std::min)(VIZ_SIZE - 1, indicator_x));
    indicator_y = (std::max)(0, (std::min)(VIZ_SIZE - 1, indicator_y));

    // マーカーを描画
    gotoxy(start_x + indicator_x, start_y + 1 + indicator_y);
    putch('O');
}

// 押されているボタンの名前を表示する関数
void print_pressed_buttons(int start_y, int button_state) {
    // ボタンの定義
    const std::vector<std::pair<ButtonMasks, const char*>> buttons = {
        {BUTTON_Y,        "Y"},
        {BUTTON_B,        "B"},
        {BUTTON_A,        "A"},
        {BUTTON_X,        "X"},
        {BUTTON_L_BUMPER, "L-Bumper"},
        {BUTTON_R_BUMPER, "R-Bumper"},
        {BUTTON_L_TRIGGER,"L-Trigger"},
        {BUTTON_R_TRIGGER,"R-Trigger"},
        {BUTTON_LS_PRESS, "LS-Press"},
        {BUTTON_RS_PRESS, "RS-Press"},
        {BUTTON_BACK,"BACK"},
        {BUTTON_START,"START"},
        {BUTTON_GUIDE,"GUIDE"},
    };

    gotoxy(1, start_y);
    printf("ボタン:          "); // タイトル行をクリア

    int current_line = start_y + 1;
    for (const auto& btn : buttons) {
        gotoxy(1, current_line);
        if (button_state & btn.first) {
            printf("- %-15s", btn.second); // 押されていれば表示
        }
        else {
            printf("%-17s", " "); // 押されていなければクリア
        }
        current_line++;
    }
}


int main() {
    clrscr(); // 画面をクリア
    setcursortype(NOCURSOR);

    for (;;) {
        gotoxy(1, 1); // 毎回カーソルを左上に戻す

        int i = inport(PJ_XPOS); printf("左スティックX:%-5d\n", i);
        int j = inport(PJ_YPOS); printf("左スティックY:%-5d\n", j);

        // ニュートラルポジションからの差分を計算
        double left_stick_x = static_cast<double>(i) - NEUTRAL_X;
        double left_stick_y = static_cast<double>(j) - NEUTRAL_Y;

        // XとYの座標からスティックの角度を計算します (度単位)
        double left_angle = atan2(left_stick_y, left_stick_x) * 180.0 / M_PI;
        printf("左スティック角度:%-5.2f度\n", left_angle);

        int k = inport(PJ_ZPOS); printf("右スティックX:%-5d\n", k);
        int l = inport(PJ_RPOS); printf("右スティックY:%-5d\n", l);

        // ニュートラルポジションからの差分を計算
        double right_stick_x = static_cast<double>(k) - NEUTRAL_X;
        double right_stick_y = static_cast<double>(l) - NEUTRAL_Y;

        // 右スティックのXとYの座標から角度を計算します (度単位)
        double right_angle = atan2(right_stick_y, right_stick_x) * 180.0 / M_PI;
        printf("右スティック角度:%-5.2f度\n", right_angle);

        int m = inport(PJ_UPOS); printf("non:          %-5d\n", m);
        int n = inport(PJ_VPOS); printf("non          :%-5d\n", n);
        int o = inport(PJ_POV);  printf("視点制御:     %-5d\n", o);

        int p = inport(PJ_BTNS);
        print_pressed_buttons(10, p); // ボタンの状態を表示する関数を呼び出し (10行目から)

        // スティックの傾きを描画
        draw_stick_visual(45, 2, left_stick_x, left_stick_y, "Left Stick");
        draw_stick_visual(45, 15, right_stick_x, right_stick_y, "Right Stick");


    }
}