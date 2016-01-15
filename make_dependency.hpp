#ifndef MAKE_DEPENDENCY_HPP_INCLUDED
#define MAKE_DEPENDENCY_HPP_INCLUDED

dependency_t* make_dependency(int id);

dependency_t* make_dependency_arm_non_eabi_gcc();
dependency_t* make_dependency_mavlink();
dependency_t* make_dependency_freertos();
dependency_t* make_dependency_stm32_stdperiph_libs();
dependency_t* make_dependency_quan();
dependency_t* make_dependency_quantracker();
#endif // MAKE_DEPENDENCY_HPP_INCLUDED
