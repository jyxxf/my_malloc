此内存池最多只能malloc MAX_MALLOC_TIMES 次
内部维护一个数据表 表中的index表示相对于memory的偏移地址 length表示长度
且index从小到大排序