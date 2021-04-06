
//使用宏和文本预处理
class Float
{
  typedef double size_type;
  static const size_type EPS = 1e-8;
private:
  size_type x;
public:
  Float(const size_type value = .0): x(value) { }
  Float& operator+=(const Float& rhs) { x += rhs.x; return *this; }
  Float& operator-=(const Float& rhs) { x -= rhs.x; return *this; }
  Float& operator*=(const Float& rhs) { x *= rhs.x; return *this; }
  Float& operator/=(const Float& rhs) { x /= rhs.x; return *this; }
};
#define ADD_ARITHMETIC_OPERATOR(x) \
inline const Float operator x(const Float& lhs, const Float& rhs)\
{\
  Float result(lhs);\
  return result x##= rhs;\
}
ADD_ARITHMETIC_OPERATOR(+)
ADD_ARITHMETIC_OPERATOR(-)
ADD_ARITHMETIC_OPERATOR(*)
ADD_ARITHMETIC_OPERATOR(/)

//不使用宏和文本预处理
class Float
{
  typedef double size_type;
  static const size_type EPS(1e-8);
private:
  size_type x;
public:
  Float(const size_type value = .0): x(value) { }
  Float& operator+=(const Float& rhs) { x += rhs.x; return *this; }
  Float& operator-=(const Float& rhs) { x -= rhs.x; return *this; }
  Float& operator*=(const Float& rhs) { x *= rhs.x; return *this; }
  Float& operator/=(const Float& rhs) { x /= rhs.x; return *this; }
};


inline const Float operator+(const Float& lhs, const Float& rhs){ Float result(lhs); return result += rhs;}
inline const Float operator-(const Float& lhs, const Float& rhs){ Float result(lhs); return result -= rhs;}
inline const Float operator*(const Float& lhs, const Float& rhs){ Float result(lhs); return result *= rhs;}
inline const Float operator/(const Float& lhs, const Float& rhs){ Float result(lhs); return result /= rhs;}
