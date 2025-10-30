#ifndef VECTOR_R1_HPP
#define VECTOR_R1_HPP

/** @namespace 项目命名空间 */
namespace pmt{
    
    /** 
     * @class   表示环\mathbb{Z}_{2^1}上的布尔数组。
     * @note    若用GPU实现本地运算，则使用32位无符号数，利于GPU高效计算；
     *          若用CPU实现本地运算，则使用64位无符号数，利于提高访问某位布尔变量的缓存命中率。
    */
    class vector_r1;

}

#endif // !VECTOR_R1_HPP