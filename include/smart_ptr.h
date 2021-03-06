/*
 * yasper - A non-intrusive reference counted pointer. 
 *	    Version: 1.04
 *			  
 *  Many ideas borrowed from Yonat Sharon and 
 *  Andrei Alexandrescu.
 *
 * (zlib license)
 * ----------------------------------------------------------------------------------	
 * Copyright (C) 2005-2007 Alex Rubinsteyn
 *
 * This software is provided 'as-is', without any express or implied
 * warranty.  In no event will the authors be held liable for any damages
 * arising from the use of this software.
 *
 * Permission is granted to anyone to use this software for any purpose,
 * including commercial applications, and to alter it and redistribute it
 * freely, subject to the following restrictions:
 *
 * 1. The origin of this software must not be misrepresented; you must not
 *    claim that you wrote the original software. If you use this software
 *    in a product, an acknowledgment in the product documentation would be
 *    appreciated but is not required.
 * 2. Altered source versions must be plainly marked as such, and must not be
 *    misrepresented as being the original software.
 * 3. This notice may not be removed or altered from any source distribution.
 *
 * -----------------------------------------------------------------------------------
 * 
 * Send all questions, comments and bug reports to:
 * Alex Rubinsteyn (alex.rubinsteyn {at-nospam} gmail {dot} com)
 */


#ifndef __SMART_POINTER_H__
#define __SMART_POINTER_H__

#include <exception>


struct NullPointerException : public std::exception
{
	NullPointerException() throw(){}
	~NullPointerException() throw(){}
	const char* what() const throw(){ return "error: attempted to dereference null pointer"; }
};

struct Counter 
{
	Counter(unsigned c = 1) : count(c){}
	unsigned count;
};

template<typename X> class smart_ptr
{
     public :
	typedef X element_type;

	// smart_ptr needs to be its own friend so smart_ptr<X> and smart_ptr<Y> can access each other's private data members
	template<class Y> friend class smart_ptr;

	// Default constructor: don't create Counter
	smart_ptr() : rawPtr(0), counter(0){}
	
	// Construct from a raw pointer
	smart_ptr( X* raw, Counter* c = 0 ) : rawPtr(0), counter(0)
	{
		if( raw )
		{
			rawPtr = raw;
			if( c ) acquire(c);
			else counter = new Counter;
		}
	}

	template<typename Y> explicit smart_ptr( Y* raw, Counter* c = 0 ) : rawPtr(0), counter(0)
	{
		if( raw )
		{
			rawPtr = static_cast<X*>(raw);
			if( c ) acquire(c);
			else counter = new Counter; 
		}
	}
	
	// Copy constructor
	smart_ptr( const smart_ptr<X> & otherPtr )
	{
		acquire( otherPtr.counter );
		rawPtr = otherPtr.rawPtr;
	}
	
	template<typename Y> explicit smart_ptr( const smart_ptr<Y> & otherPtr ) : rawPtr(0), counter(0)
	{
		acquire(otherPtr.counter);
		rawPtr = static_cast<X*>(otherPtr.GetRawPointer());
	}

	// Destructor
	~smart_ptr()
	{
		release();
	}

	// Assignment to another smart_ptr
	smart_ptr & operator = ( const smart_ptr<X> & otherPtr )
	{
		if( this != &otherPtr )
		{
			release();
			acquire(otherPtr.counter);
			rawPtr = otherPtr.rawPtr;
		}

		return *this;
	}

	template<typename Y> smart_ptr & operator = ( const smart_ptr<Y> & otherPtr )
	{
		if( this != (smart_ptr<X>*)&otherPtr )
		{
			release();
			acquire(otherPtr.counter);
			rawPtr = static_cast<X*>(otherPtr.GetRawPointer());
		}

		return *this;
	}

	// Assignment to raw pointers is really dangerous business. If the raw pointer is also being used elsewhere,
	// we might prematurely delete it, causing much pain. Use sparingly/with caution.
	smart_ptr & operator = ( X* raw )
	{
		if( raw )
		{
			release(); 
			counter = new Counter; 
			rawPtr  = raw; 
		}

		return *this;
	}

	template<typename Y> smart_ptr & operator = ( Y* raw )
	{
		if( raw )
		{
			release();
			counter = new Counter; 
			rawPtr  = static_cast<X*>(raw);
		}

		return *this;
	}

	// Assignment to long to allow smart_ptr<X> = NULL, also allows raw pointer assignment by conversion. 
	// Raw pointer assignment is really dangerous! If the raw pointer is being used elsewhere, it will get deleted prematurely.
	smart_ptr & operator = ( long num )
	{
		if( num == 0 ) // pointer set to null
		{
			release();
		}
		else // assign raw pointer by conversion
		{
			release();
			counter = new Counter;
			rawPtr  = reinterpret_cast<X*>(num);
		}

		return *this;
	}

	// Member Access
	X* operator->() const
	{
		return GetRawPointer();
	}

	// Dereference the pointer
	X & operator * () const 
	{
		return *GetRawPointer();
	}

	// Conversion/casting operators
	operator bool () const
	{
		return IsValid();
	}

	// Implicit casts to base types of the pointer we're storing
	template<typename Y> operator Y* () const
	{
		return static_cast<Y*>(rawPtr);
	}

	template<typename Y> operator const Y* () const
	{
		return static_cast<const Y*>(rawPtr);
	}

	template<typename Y> operator smart_ptr<Y>()
	{
		// new smart_ptr must also take our counter or else the reference counts will go out of sync
		return smart_ptr<Y>(rawPtr, counter);
	}

	// Provide access to the raw pointer
	X* GetRawPointer() const         
	{
		if( rawPtr == 0 ) throw new NullPointerException;
		return rawPtr;
	}
	
	// Is there only one reference on the counter?
	bool IsUnique() const
	{
		if( counter && counter->count == 1 ) return true;
		return false;
	}

	bool IsValid() const
	{
		if( counter && rawPtr ) return true;
		return false;
	}

	unsigned GetCount() const
	{
		if( counter ) return counter->count;
		return 0;
	}

     private :
	X* rawPtr;
	Counter* counter;

	// increment the count
	void acquire( Counter* c )
	{
		counter = c;

		if( c )
			(c->count)++;
	}

	// decrement the count, delete if it is 0
	void release()
	{
		if( counter )
		{
			(counter->count)--;

			if( counter->count == 0 )
			{
				delete counter;
				delete rawPtr;
			}
		}

		counter = 0;
		rawPtr  = 0;
	}
};

template<typename X, typename Y> bool operator == ( const smart_ptr<X> & a, const smart_ptr<Y> & b ) 
{
	return a.GetRawPointer() == b.GetRawPointer(); 
}

template<typename X, typename Y> bool operator == ( const smart_ptr<X> & a, Y* raw ) 
{
	return a.GetRawPointer() == raw;
}

template<typename X> bool operator == ( const smart_ptr<X> & a, long num )
{
	if( num == 0 && !a.IsValid() ) //both pointer and address are null
	{
		return true; 
	}
	else //convert num to a pointer, compare addresses
	{
		return a == reinterpret_cast<X*>(num);
	}
} 

template<typename X, typename Y> bool operator != ( const smart_ptr<X> & a, const smart_ptr<Y> & b )
{
	return !operator == (a, b);
}

template<typename X, typename Y> bool operator!=(const smart_ptr<X> & a, Y* raw )
{
	return !operator == (a, raw);
}

template<typename X> bool operator != ( const smart_ptr<X> & a, long num )
{
	return !operator == (a, num);
}

template<typename X, typename Y> bool operator && ( const smart_ptr<X> & a, const smart_ptr<Y> & b )
{
	return a.IsValid() && b.IsValid();
}

template<typename X> bool operator && ( const smart_ptr<X> & a, bool rval )
{
	return a.IsValid() && rval;
}

template<typename X> bool operator && ( bool lval, const smart_ptr<X> & b )
{
	return lval && b.IsValid();
}

template<typename X, typename Y> bool operator || ( const smart_ptr<X> & a, const smart_ptr<Y> & b )
{
	return a.IsValid() || b.IsValid();
}

template<typename X> bool operator || ( const smart_ptr<X> & a, bool rval )
{
	return a.IsValid() || rval;
}

template<typename X> bool operator || ( bool lval, const smart_ptr<X> & b )
{
	return lval || b.IsValid(); 
}

template<typename X> bool operator ! ( const smart_ptr<X> & p )
{
	return !p.IsValid();
}

/* less than comparisons for storage in containers */
template<typename X, typename Y> bool operator < (const smart_ptr<X> & a, const smart_ptr<Y> & b )
{
	return a.GetRawPointer() < b.GetRawPointer();
}

template<typename X, typename Y> bool operator < ( const smart_ptr<X> & a, Y* raw )
{
	return a.GetRawPointer() < raw;
}

template<typename X, typename Y> bool operator < ( X* raw, const smart_ptr<Y> & b )
{
	return raw < b.GetRawPointer();
}
#endif //__SMART_POINTER_H__
