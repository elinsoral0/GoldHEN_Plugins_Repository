#include "ps4.h"

// تعريف هيكل اللون الخاص بمكتبة libScePad
typedef struct ScePadColor {
    uint8_t r;
    uint8_t g;
    uint8_t b;
    uint8_t a;
} ScePadColor;

// دالة تحويل تدرج الألوان (Hue) إلى قيم RGB
void hsvToRgb(int hue, uint8_t *r, uint8_t *g, uint8_t *b) {
    int region = hue / 43;
    int remainder = (hue - (region * 43)) * 6; 
    int p = 0;
    int q = (255 * (255 - remainder)) >> 8;
    int t = (255 * remainder) >> 8;

    switch (region) {
        case 0:  *r = 255; *g = t;   *b = p;   break;
        case 1:  *r = q;   *g = 255; *b = p;   break;
        case 2:  *r = p;   *g = 255; *b = t;   break;
        case 3:  *r = p;   *g = q;   *b = 255; break;
        case 4:  *r = t;   *g = p;   *b = 255; break;
        default: *r = 255; *g = p;   *b = q;   break;
    }
}

int _main(struct thread *td) {
    // تهيئة المكتبات الأساسية
    initKernel();
    initLibc();
    initPthread();

    // تحميل مكتبة التحكم في القبضة ومكتبة المستخدمين
    int padLib = sceKernelLoadStartModule("libScePad.sprx", 0, NULL, 0, NULL, NULL);
    int userLib = sceKernelLoadStartModule("libSceUserService.sprx", 0, NULL, 0, NULL, NULL);

    // تعريف مؤشرات الدوال من النظام
    int (*scePadInit)(void);
    int (*scePadOpen)(int userId, int type, int index, void *param);
    int (*scePadSetLightBar)(int handle, ScePadColor *pColor);
    int (*sceUserServiceGetInitialUser)(int *userId);

    RESOLVE(padLib, scePadInit);
    RESOLVE(padLib, scePadOpen);
    RESOLVE(padLib, scePadSetLightBar);
    RESOLVE(userLib, sceUserServiceGetInitialUser);

    // بدء تشغيل خدمة القبضة وجلب معرف المستخدم الأول
    scePadInit();
    int userId;
    sceUserServiceGetInitialUser(&userId);
    int padHandle = scePadOpen(userId, 0, 0, NULL);

    ScePadColor color;
    color.a = 255; // أقصى درجة سطوع
    int hue = 0;

    // حلقة تكرارية مستمرة لتغيير الألوان
    while (1) {
        hsvToRgb(hue, &color.r, &color.g, &color.b);
        scePadSetLightBar(padHandle, &color);
        
        hue = (hue + 1) % 256;
        sceKernelUsleep(20000); // سرعة التغيير (20 مللي ثانية)
    }

    return 0;
}
