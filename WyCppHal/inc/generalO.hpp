#ifndef __WY_LIB_CPP_GEN_OUT_H__
#define __WY_LIB_CPP_GEN_OUT_H__
#include "vector"
namespace genO
{
    // struct ChooseLine
    class generalOutputBase
    {
    protected:
        const uint8_t *font;
        uint8_t asciiHigh, asciiWide, fontUnitSize;
        uint8_t zh_High, zh_Wide;
        uint8_t maxWord = 16, maxLine = 4;
        uint8_t line = 0, wordCount = 0;
        virtual void char_display(const uint8_t *ptr_pic, bool contrast, uint8_t l, uint8_t word) = 0;
        void char_display(const uint8_t *ptr_pic, bool contrast)
        {
            char_display(ptr_pic, contrast, this->line, this->wordCount);
        }
        void clearPlaceHolder(void);

    private:
        std::vector<std::vector<uint8_t> > placeHolder;
        uint16_t (*loadZH)(uint32_t add, uint8_t *buf, uint16_t len);

    public:
        class // ChooseLine
        {
        private:
            char val = -1;
            uint8_t step = 2;

        public:
            void setStep(uint8_t step) { this->step = step; };
            bool operator==(uint8_t v) { return this->val == v; }
            bool operator!=(uint8_t v) { return this->val != v; }
            void operator++(void) { this->val += this->step; }
            void operator--(void) { this->val -= this->step; }
            void disable(void) { this->val = -1; }
            void reset(void) { this->val = 0; }
        } chooseLine;
        generalOutputBase() = default;
        void loadFont(const uint8_t *font, uint8_t asciiHigh, uint8_t asciiWide);
        void setScreenSize(uint8_t width, uint8_t height);
        void putChar(char c);
        void putNum(__IO int num);
        void putNum(__IO int num, uint8_t decimalPlaces);
        void print(char *s);
        void placeFill(char *);
        // void left(void);
        // void left(uint8_t len);
        // void up(void);
        // void up(uint8_t len);
        // void down(void);
        // void down(uint8_t len);
        // void right(void);
        // void right(uint8_t len);
        virtual void clear(void) = 0;
        // ~generalOutputBase();
    };
} // namespace genO
#endif /* __WY_LIB_CPP_GEN_OUT_H__ */
