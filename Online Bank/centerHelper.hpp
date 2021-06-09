#include <iostream>
#include <string>
#include <iomanip>

// A new thin wrapper around a string of characters
template<typename charT, typename traits = std::char_traits<charT> >
class center_helper {
   private:
      basic_string<charT, traits> _str;

   public:
      center_helper(basic_string<charT, traits> str) : _str(str) {}

      template<typename a, typename b>
      friend basic_ostream<a,b>& operator<<(basic_ostream<a,b>& s,
         const center_helper<a, b>& c);
};

// Allows us to center things that are convertible to a string
template<typename charT, typename traits = char_traits<charT> >
center_helper<charT, traits> centered(basic_string<charT, traits> str) {
   return center_helper<charT, traits> (str);
}

// Allows us to center strings
center_helper<string::value_type, string::traits_type> centered(const string& str) {
   // Pulls out the value type and the traits type of the string being passed
   return center_helper<string::value_type, string::traits_type>(str);
}

// Does the centering
template<typename charT, typename traits>
basic_ostream<charT, traits>& operator<<(basic_ostream<charT, traits>& os,
   const center_helper<charT, traits>& c) {

   std::streamsize streamWidth = os.width();

   if(streamWidth > (int)c._str.length()) {
      std::streamsize left = (streamWidth + c._str.length()) / 2;
      os.width(left);
      os << c._str;
      os.width(streamWidth - left);
      os << "";
   }
   else {
      os << c._str;
   }

   return os;
}






