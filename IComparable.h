/**
 * @brief an interface for one type of valid rbtree key
 */
class IComparable {
public:
    /**
     * @brief Get the int representation of the object
     *
     * @return int representation
     */
    virtual int getIntRep() = 0;
    /**
     * @brief the subtraction operator
     *
     * @param[in] rhs the right hand side object
     * @return int 0 if equal, positive if this is greater, negative if rhs is greater
     */
    virtual int operator-(IComparable *rhs) = 0;
    /**
     * @brief logical equals
     *
     * @param[in] rhs the right hand side object
     * @return true if equal to rhs
     * @return false otherwise
     */
    virtual bool operator==(IComparable *rhs) = 0;
};