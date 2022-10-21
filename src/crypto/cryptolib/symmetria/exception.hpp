

#include <exception>
class FileOpenError : virtual public std::exception
{
protected:
    int error_number;
    int error_offset;
    std::string error_message; ///< Error message
public:
    /** Constructor (C++ STL string, int, int).      
*  @param msg The error message      
*  @param err_num Error number      
*  @param err_off Error offset      
*/
    explicit FileOpenError(const std::string &msg) : error_message(msg) {}
    /** Destructor.      
*  Virtual to allow for subclassing.      
*/
    virtual ~FileOpenError() throw() {}
    /** Returns a pointer to the (constant) error description.      
*  @return A pointer to a const char*. The underlying memory      
*  is in possession of the Except object. Callers must      
*  not attempt to free the memory.      
*/
    virtual const char *what() const throw()
    {
        return error_message.c_str();
    }
    /** Returns error number.      
*  @return #error_number      
*/
    virtual int getErrorNumber() const throw() { return error_number; }
    /**Returns error offset.      
* @return #error_offset      
*/
    virtual int getErrorOffset() const throw() { return error_offset; }
};