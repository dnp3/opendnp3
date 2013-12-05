namespace openpal
{

/**
* This is a bit of a hack to avoid copying large objects with lamdas
* borrowed from here:
* http://marcoarena.wordpress.com/2012/11/01/learn-how-to-capture-by-move/
*
* This should be unnecessary in C++14
*
*/
template<typename T>
struct move_on_copy
{
   move_on_copy(T&& aValue) : value(std::move(aValue)) {}
   move_on_copy(const move_on_copy& other) : value(std::move(other.value)) {}
 
   T& Value()
   {
      return value;
   }
 
   const T& Value() const
   {
      return value;
   }
 
private:
   mutable T value;
   move_on_copy& operator=(move_on_copy&& aValue); // not needed here
   move_on_copy& operator=(const move_on_copy& aValue); // not needed here
};

}
