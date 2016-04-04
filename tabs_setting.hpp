#ifndef TABS_SETTING_HPP_INCLUDED
#define TABS_SETTING_HPP_INCLUDED

#include <iostream>

struct tabs_setting_t{

tabs_setting_t(int n):  m_cur_tab{0},m_tab_size{n}{}
   void incr(int n = 1)
   {
      m_cur_tab += n;
   }
   void decr( int n = 1){
      m_cur_tab -= n;
      if(m_cur_tab < 0){
         m_cur_tab = 0;
      }
   }
   
   std::ostream & output(std::ostream & os) const
   {
      for ( int i = 0; i < m_cur_tab ; ++i){
         for( int j = 0; j < m_tab_size; ++j){
            os << ' ';
         }
      }
      return os;
   }

   
private:
   int m_cur_tab;
   int const m_tab_size;
};

inline std::ostream & operator << ( std::ostream & os, tabs_setting_t const & tabs)
{
   tabs.output(os);
   return os;
}


extern tabs_setting_t tabs;
inline void push_fun(std::string const & fun_name, std::string const & args = "")
{
   std::cout << tabs << fun_name << "(" << args << ")\n" << tabs << "{\n";
   tabs.incr();
}

inline void pop_fun()
{
   tabs.decr();
   std::cout << tabs <<"}\n";
}

inline void statement(std::string const & str)
{
   std::cout << tabs << str << '\n';
}



#endif // TABS_SETTING_HPP_INCLUDED
