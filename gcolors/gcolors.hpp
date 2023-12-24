#ifndef CTUI_COLORS_HPP
#define CTUI_COLORS_HPP

#include <cstdint>
#include <algorithm>
#include <string>
#include <format>
#include <cmath>
using std::clamp;

#include "../gutils/gutils.hpp"
#include "../gmath/math.hpp"

// TODO: WORK OUT ACCESS TO FIELDS

namespace gcolors {
    static constexpr uint8_t operator""_u8(unsigned long long value) {
        return value <= std::numeric_limits<uint8_t>::max() ? value : std::numeric_limits<uint8_t>::max();
    }

    static constexpr int I_MIN = 0;
    static constexpr int I_MAX = 255;
    static constexpr float F_MIN = 0.0f;
    static constexpr float F_MAX = 1.0f;

    static constexpr int HSL_HUE_MAX = 359;
    static constexpr int HSL_LUMSAT_MAX = 100;

    template<gutils::floating_point F>
    static constexpr int floatToInt(const F value) {
        return (int)gmath::gRound(value * 255.0f);
    }

    template<gutils::integer I>
    static constexpr float intToFloat(const I value) {
        return float(value) / 255.0f;
    }

    template<gutils::number T>
    class rgb;

    typedef rgb<float> rgb_f;
    typedef rgb<int16_t> rgb_i;
    typedef rgb<uint16_t > rgb_ui;

    template<gutils::number T>
    class rgba;

    typedef rgba<float> rgba_f;
    typedef rgba<int8_t> rgba_i;
    typedef rgba<uint8_t> rgba_ui;

    template <gutils::number T>
    class color {
    public:
        color() = default;

        [[nodiscard]] virtual inline rgb_f to_rgb_f() const;
        [[nodiscard]] virtual inline rgb_i to_rgb_i() const;
        [[nodiscard]] virtual inline rgb_ui to_rgb_ui() const;

        [[nodiscard]] virtual inline std::string to_string() const;

        [[nodiscard]] virtual inline T& operator[](const size_t index);
        [[nodiscard]] virtual inline const T& operator[](const size_t index) const;
    };

    template <gutils::number T>
    class rgb : color<T> {
    private:
        /*template<gutils::number N>
        void setTypeSafe(const N nr, const N ng, const N nb, const N na)
        requires(gutils::unsigned_integer<T> && (gutils::floating_point<N> || gutils::signed_integer<N>)) {
            this->r = nr < 0 ? 0 : nr > I_MAX ? I_MAX : (T)nr;
            this->g = ng < 0 ? 0 : ng > I_MAX ? I_MAX : (T)ng;
            this->b = nb < 0 ? 0 : nb > I_MAX ? I_MAX : (T)nb;
            this->a = na < 0 ? 0 : na > I_MAX ? I_MAX : (T)na;
        }*/


        /*template <gutils::number N>
        void set_SizeOfCast(const N nr, const N ng, const N nb, const N na) {
            if (sizeof(T) > sizeof(N))
                set((T) nr, (T) ng, (T) nb, (T) na);
            else
                set(nr, ng, nb, na);
        }

        template <gutils::number N>
        void set_ScaleCast(const N nr, const N ng, const N nb, const N na, float scalar) {
            set((T) (nr * scalar), (T) (ng * scalar), (T) (nb * scalar), (T) (na * scalar));
        }

        template <gutils::number N>
        void set_TypeSafe(const N nr, const N ng, const N nb, const N na) {
            if (gutils::floating_point<T>) {
                if (gutils::floating_point<N>) {
                    set_SizeOfCast(nr, ng, nb, na);
                } else {
                    set_ScaleCast(nr, ng, nb, na, 1.0f / I_MAX);
                }
            } else if (gutils::floating_point<N>) {
                set_ScaleCast(nr, ng, nb, na, I_MAX);
            } else {
                set_SizeOfCast(nr, ng, nb, na);
            }
        }*/
    public:
        T r;
        T g;
        T b;

        rgb() {
            setMax();
        }

        template <gutils::number N>
        explicit rgb(const rgb<N>& old) : rgb(old.r, old.g, old.b) { }

        template<gutils::number N>
        rgb(const N r, const N g, const N b) {
            set(r, g, b);
        }

        template<gutils::number N>
        rgb& set(const N nr, const N ng, const N nb) requires(std::is_same_v<T, N>) {
            rawSet(nr, ng, nb);
            return *this;
        }

        template<gutils::floating_point F>
        rgb& set(const F r, const F g, const F b) requires(gutils::integer<T>) {
            this->r = gmath::gClamp(I_MIN, I_MAX, floatToInt(r));
            this->g = gmath::gClamp(I_MIN, I_MAX, floatToInt(g));
            this->b = gmath::gClamp(I_MIN, I_MAX, floatToInt(b));
            return *this;
        }

        template<gutils::integer I>
        rgb& set(const I r, const I g, const I b) requires(gutils::floating_point<T>) {
            this->r = gmath::gClamp(F_MIN, F_MAX, intToFloat(r));
            this->g = gmath::gClamp(F_MIN, F_MAX, intToFloat(g));
            this->b = gmath::gClamp(F_MIN, F_MAX, intToFloat(b));
            return *this;
        }

        template<gutils::integer I>
        rgb& set(const I r, const I g, const I b) requires(gutils::integer<T> && !std::is_same_v<T, I>) {
            this->r = (T)(gmath::gClamp(I_MIN, I_MAX, (int)r));
            this->g = (T)(gmath::gClamp(I_MIN, I_MAX, (int)g));
            this->b = (T)(gmath::gClamp(I_MIN, I_MAX, (int)b));
            return *this;
        }

        template<gutils::floating_point F>
        rgb& set(const F r, const F g, const F b) requires(gutils::floating_point<T> && !std::is_same_v<T, F>) {
            this->r = (T) (gmath::gClamp(F_MIN, F_MAX, (float)r));
            this->g = (T) (gmath::gClamp(F_MIN, F_MAX, (float)g));
            this->b = (T) (gmath::gClamp(F_MIN, F_MAX, (float)b));
            return *this;
        }

        template<gutils::number N>
        rgb& set(const rgb<N>& color) {
            return set(color.r, color.g, color.b, color.a);
        }

        template<gutils::number N>
        rgb& set(const N value) {
            return set(value, value, value, value);
        }

        template<gutils::number N>
        void rawSet(const N nr, const N ng, const N nb) {
            this->r = nr;
            this->g = ng;
            this->b = nb;
        }

        void rawSet(const T value) {
            rawSet(value, value, value);
        }

        rgb& setMax() requires(gutils::floating_point<T>) {
            rawSet(F_MAX);
            return *this;
        }

        rgb& setMax() requires(gutils::integer<T>) {
            rawSet(I_MAX);
            return *this;
        }

        rgb& setMin() {
            rawSet(0);
            return *this;
        }

        [[nodiscard]] inline rgb_f to_rgb_f() const override {
            return rgb_f{*this};
        }

        [[nodiscard]] inline rgb_i to_rgb_i() const override {
            return rgb_i{*this};
        }

        [[nodiscard]] inline rgb_ui to_rgb_ui() const override {
            return rgb_ui{*this};
        }

        [[nodiscard]] inline T& operator[](const size_t index) override {
            switch (index) {
                case 0: return r;
                case 1: return g;
                case 2: return b;
                default: throw std::invalid_argument("Index used while indexing into rgb cannot be greater than 3.");
            }
        }

        [[nodiscard]] inline const T& operator[](const size_t index) const override {
            switch (index) {
                case 0: return r;
                case 1: return g;
                case 2: return b;
                default: throw std::invalid_argument("Index used while indexing into rgb cannot be greater than 3.");
            }
        }

        [[nodiscard]] std::string to_string() const override {
            return std::move(std::format("rgb<{}>[{}, {}, {}]", typeid(T).name(), r, g, b));
        }
    };

    template <gutils::number T>
    class rgba : color<T> {
    private:
    public:
        T r;
        T g;
        T b;
        T a;

        rgba() {
            setMax();
        }

        template <gutils::number N>
        explicit rgba(const rgba<N>& old) : rgba(old.r, old.g, old.b, old.a) { }

        template<gutils::number N>
        rgba(const N r, const N g, const N b, const N a) {
            set(r, g, b, a);
        }

        template<gutils::number N>
        rgba& set(const N nr, const N ng, const N nb, const N na) requires(std::is_same_v<T, N>) {
            rawSet(nr, ng, nb, na);
            return *this;
        }

        template<gutils::floating_point F>
        rgba& set(const F r, const F g, const F b, const F a) requires(gutils::integer<T>) {
            this->r = gmath::gClamp(I_MIN, I_MAX, floatToInt(r));
            this->g = gmath::gClamp(I_MIN, I_MAX, floatToInt(g));
            this->b = gmath::gClamp(I_MIN, I_MAX, floatToInt(b));
            this->a = gmath::gClamp(I_MIN, I_MAX, floatToInt(a));
            return *this;
        }

        template<gutils::integer I>
        rgba& set(const I r, const I g, const I b, const I a) requires(gutils::floating_point<T>) {
            this->r = gmath::gClamp(F_MIN, F_MAX, intToFloat(r));
            this->g = gmath::gClamp(F_MIN, F_MAX, intToFloat(g));
            this->b = gmath::gClamp(F_MIN, F_MAX, intToFloat(b));
            this->a = gmath::gClamp(F_MIN, F_MAX, intToFloat(a));
            return *this;
        }

        template<gutils::integer I>
        rgba& set(const I r, const I g, const I b, const I a) requires(gutils::integer<T> && !std::is_same_v<T, I>) {
            this->r = (T)(gmath::gClamp(I_MIN, I_MAX, (int)r));
            this->g = (T)(gmath::gClamp(I_MIN, I_MAX, (int)g));
            this->b = (T)(gmath::gClamp(I_MIN, I_MAX, (int)b));
            this->a = (T)(gmath::gClamp(I_MIN, I_MAX, (int)a));
            return *this;
        }

        template<gutils::floating_point F>
        rgba& set(const F r, const F g, const F b, const F a) requires(gutils::floating_point<T> && !std::is_same_v<T, F>) {
            this->r = (T) (gmath::gClamp(F_MIN, F_MAX, (float)r));
            this->g = (T) (gmath::gClamp(F_MIN, F_MAX, (float)g));
            this->b = (T) (gmath::gClamp(F_MIN, F_MAX, (float)b));
            this->a = (T) (gmath::gClamp(F_MIN, F_MAX, (float)a));
            return *this;
        }

        template<gutils::number N>
        rgba& set(const rgba<N>& color) {
            return set(color.r, color.g, color.b, color.a);
        }

        template<gutils::number N>
        rgba& set(const N value) {
            return set(value, value, value, value);
        }

        template<gutils::number N>
        void rawSet(const N nr, const N ng, const N nb, const N na) {
            this->r = nr;
            this->g = ng;
            this->b = nb;
            this->a = na;
        }

        void rawSet(const T value) {
            rawSet(value, value, value, value);
        }

        rgba& setMax() requires(gutils::floating_point<T>) {
            rawSet(F_MAX);
            return *this;
        }

        rgba& setMax() requires(gutils::integer<T>) {
            rawSet(I_MAX);
            return *this;
        }

        rgba& setMin() {
            rawSet(0);
            return *this;
        }

        [[nodiscard]] inline rgb_f to_rgb_f() const override {
            return rgb_f{r, g, b};
        }

        [[nodiscard]] inline rgb_i to_rgb_i() const override {
            return rgb_i{r, g, b};
        }

        [[nodiscard]] inline rgb_ui to_rgb_ui() const override {
            return rgb_ui{r, g, b};
        }

        [[nodiscard]] inline T& operator[](const size_t index) override {
            switch (index) {
                case 0: return r;
                case 1: return g;
                case 2: return b;
                case 3: return a;
                default: throw std::invalid_argument("Index used while indexing into rgb cannot be greater than 3.");
            }
        }

        [[nodiscard]] inline const T& operator[](const size_t index) const override {
            switch (index) {
                case 0: return r;
                case 1: return g;
                case 2: return b;
                case 3: return a;
                default: throw std::invalid_argument("Index used while indexing into rgb cannot be greater than 3.");
            }
        }

        [[nodiscard]] std::string to_string() const override {
            return std::move(std::format("rgba<{}>[{}, {}, {}, {}]", typeid(T).name(), r, g, b, a));
        }
    };

    template<gutils::number T>
    [[nodiscard]] rgb_f color<T>::to_rgb_f() const {
        throw std::runtime_error {"Base color::to_rgb_f called."};
    }

    template<gutils::number T>
    [[nodiscard]] rgb_i color<T>::to_rgb_i() const {
        throw std::runtime_error {"Base color::to_rgb_f called."};
    }

    template<gutils::number T>
    [[nodiscard]] rgb_ui color<T>::to_rgb_ui() const {
        throw std::runtime_error{"Base color::to_string called."};
    }

    template<gutils::number T>
    [[nodiscard]] T& color<T>::operator[](const size_t index) {
        throw std::runtime_error{"Base color::operator[] called."};
    }

    template<gutils::number T>
    [[nodiscard]] const T& color<T>::operator[](const size_t index) const {
        throw std::runtime_error{"Base color::operator[] called."};
    }

    template<gutils::number T>
    [[nodiscard]] std::string color<T>::to_string() const {
        throw std::runtime_error{"Base color::to_string called."};
    }

    constexpr static inline float HueToFloat(int value) {
        if (value == HSL_HUE_MAX)
            return F_MAX;
        else if (value == 0)
            return 0;

        return ((float) value / (float) HSL_HUE_MAX);
    }

    constexpr static inline int HueFToRgb(float p, float q, float t) {
        std::cout << std::format("pqt: {} {} {}", p, q, t) << std::endl;
        if (t < 0)
            t += 1;

        if (t > 1)
            t -= 1;

        if ((6 * t) < 1)
            return (int) (p + (q - p) * 6 * t);

        if ((2 * t) < 1)
            return (int) q;

        if ((3 * t) < 2)
            return (int) (p + (q - p) * ((2.0f / 3) - t) * 6);
    }

    constexpr static inline float PercentToFloat(int percent) {
        return (float) percent / 100;
    }

} // gcolors

#endif //CTUI_COLORS_HPP
