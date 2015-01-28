# jsoncons: a C++ library for json construction

`jsoncons` is a C++ library for the construction of [JavaScript Object Notation (JSON)](http://www.json.org). It supports parsing a JSON file or string into a tree structured `json` value, building a `json` value in C++ code, and serializing a `json` value to a file or string. It also provides an event-based API for reading and writing JSON documents that are too large to fit into available memory, somewhat analogously to SAX processing in the XML world. Documentation may be found in https://github.com/danielaparker/jsoncons/wiki.

jsoncons uses some features that are new to C++ 11, particularly move semantics, however, it has been written to be compatible with VC++ 10 SP1 (note that SP1 is required for VC++ 10, it fixes compiler bugs with move semantics.) It has been tested with MS Visual C++ 10 SP1, Intel C++ Studio XE 2013, clang 3.3 and GCC 4.8. 

The code repository is on [github](https://github.com/danielaparker/jsoncons). It is distributed under the [Boost Software License](http://www.boost.org/users/license.html)

The library has a number of features, which are listed below:

- Uses the standard C++ input/output streams library
- Implements parsing and serializing JSON text in UTF-8 for narrow character strings and streams
- Supports UTF16 (UTF32) encodings with size 2 (size 4) wide characters
- Correctly handles surrogate pairs in \uXXXX escape sequences
- Supports event based JSON parsing and serializing with user defined input and output handlers
- Guarantees basic exception safety (no leaks)
- Accepts and ignores single line comments that start with //, and multi line comments that start with /* and end with */
- Supports optional escaping of the solidus (/) character
- Supports Nan, Inf and -Inf replacement
- Supports reading multiple json objects from a stream
- Supports optional escaping of non-ascii UTF-8 octets
- Allows extensions to the types accepted by the json class accessors and modifiers
- Supports storing "any" values in a json object or array, with specialized serialization
- Supports reading (writing) JSON values from (to) CSV files

## Using the code

The jsoncons library is header-only: it consists solely of header files containing templates and inline functions, and requires no separately-compiled library binaries when linking. It has no dependence on other libraries.

To install the jsoncons library, download the zip file, unpack the release, under `src` find the directory `jsoncons`, and copy it to your `include` directory. If you wish to use extensions, copy the `jsoncons_ext` directory as well. 

The jsoncons classes and functions are in namespace jsoncons. The following using declarations are for the examples that appear below.

    #include "jsoncons/json.hpp"

    using std::string;
    using std::cout;
    using std::endl;
    using jsoncons::null_type;
    using jsoncons::json;

### Reading json values from a file

Here is a sample file (books.json):

    [
        {
            "title" : "Kafka on the Shore",
            "author" : "Haruki Murakami",
            "price" : 25.17
        },
        {
            "title" : "Women: A Novel",
            "author" : "Charles Bukowski",
            "price" : 12.00
        },
        {
            "title" : "Cutter's Way",
            "author" : "Ivan Passer"
        }
    ]

You can read the file into a json value like this

    json books = json::parse_file("books.json");

You can then loop through the books

    for (size_t i = 0; i < books.size(); ++i)
    {
        try
        {
            json& book = books[i];
            string author = book["author"].as<string>();
            string title = book["title"].as<string>();
            double price = book["price"].as<double>();
            cout << author << ", " << title << ", " << price << endl;
        }
        catch (const json_exception& e)
        {
            cerr << e.what() << endl;
        }
    }

The output is

    Haruki Murakami, Kafka on the Shore, 25.17
    Charles Bukowski, Women: A Novel, 12
    Member price not found.
    Note that the third book is missing a price, which causes an exception to be thrown.

You have a choice of accessors:

    book["price"] will throw if there is no price
    book.get("price") will return json::null if there is no price
    book.get("price",default_value) will return default_value if there is no price

So if you want to show "n/a" for the missing price, you can use this accessor

    string price = book.get("price","n/a").as<string>();

and the output becomes

    Haruki Murakami, Kafka on the Shore, 25.17
    Charles Bukowski, Women: A Novel, 12.0
    Ivan Passer, Cutter's Way, n/a

Or you can check if book has a member "price" with the method `has_member`, and output accordingly,

    if (book.has_member("price"))
    {
        double price = book["price"].as<double>();
        cout << price;
    }
    else
    {
        cout << "n/a";
    }

### Constructing json values in C++

To construct an empty json object, use the default constructor:

     json image_sizing;

Serializing it to standard out

    cout << image_sizing << endl;

produces

    {}

Adding some members,

    image_sizing["resize_to_fit"] = true;  // a boolean 
    image_sizing["resize_unit"] =  "pixels";  // a string
    image_sizing["resize_what"] =  "long_edge";  // a string
    image_sizing["dimension1"] = 9.84;  // a double
    image_sizing["dimension2"] = null_type();  // a null

Serializing it, this time with pretty print,

 	cout << pretty_print(image_sizing) << endl;

produces

    {
        "dimension1":9.84,
        "dimension2":null,
        "resize_to_fit":true,
        "resize_unit":"pixels",
        "resize_what":"long_edge"
    }

To construct a json array, use the copy constructor with a prototype json array:

    json image_formats(json::an_array);

Adding some elements,

    image_formats.add("JPEG");
    image_formats.add("PSD");
    image_formats.add("TIFF");
    image_formats.add("DNG");

Combining the two

    json file_export;
    file_export["image_formats"] = std::move(image_formats);
    file_export["image_sizing"] = std::move(image_sizing);

and serializing

    cout << pretty_print(file_export) << endl;
produces

    {
        "image_formats":
        ["JPEG","PSD","TIFF","DNG"],
        "image_sizing":
        {
            "dimension1":9.84,
            "dimension2":null,
            "resize_to_fit":true,
            "resize_unit":"pixels",
            "resize_what":"long_edge"
        }
    }

### Iterators

jsoncons::json supports iterators for accessing the members of json objects and the elements of json arrays.

An example of iterating over the name-value pairs of a json object:

    json person;
    person["first_name"] = "Jane";
    person["last_name"] = "Roe";
    person["events_attended"] = 10;
    person["accept_waiver_of_liability"] = true;

    for (auto it = person.begin_members(); it != person.end_members(); ++it)
    {
        cout << "name=" << it->name() << ", value=" << it->value().as<string>() << endl;
    }

An example of iterating over the elements of a json array:

    json cities(json::an_array);
    cities.add("Montreal");
    cities.add("Toronto");
    cities.add("Ottawa");
    cities.add("Vancouver");

    for (auto it = cities.begin_elements(); it != cities.end_elements(); ++it)
    {
        cout << it->as<string>() << endl;
    }

## About jsoncons::json

The json class is an instantiation of the basic_json class template that uses char as the character type,

    typedef basic_json<char,storage<char>> json

The library includes an instantiation for wide characters as well,

    typedef basic_json<wchar_t,storage<wchar_t>> wjson

Note that the template parameter storage is a place holder for what in a future release will allow the user to have control over internal storage and memory allocation.

## Type Extensibility

In the json class, accessors and modifiers are templated, for example,

    template<typename T>
    bool is() const

    template<typename T>
    T as() const

    template <typename T>
    basic_json& operator=(T val)

The implementations of these functions and operators make use of the class template value_adapter

    template <typename Char, typename Storage, typename T>
    class value_adapter
    {
    public:
        bool is(const basic_json& val) const {return false;}
        T as(const basic_json& val) const;
        void assign(basic_json& self, const T val);
    };

This class template is extensible, you as a user can extend `value_adapter` in the `jsoncons` namespace with your own types. You can, for example, extend `value_adapter` to access and modify `json` structures with `boost::gregorian::date values`, and in your code, write

    json deal;
    deal["maturity"] = boost::gregorian::date(2015,1,1);
	
    json observation_dates(json::an_array);
    observation_dates.add(boost::gregorian::date(2013,10,21));
    observation_dates.add(boost::gregorian::date(2013,10,28));
    deal["observation_dates"] = std::move(observation_dates);
	
    boost::gregorian::date maturity = deal["maturity"].as<boost::gregorian::date>();
	
    cout << deal << endl;	

producing

    {
        "maturity":"2015-01-01",
        "observation_dates":
        ["2013-10-21","2013-10-28"]
    }

## json any

jsoncons provides a class `json::any` that can contain a value of 
any type as long as that type supports copy construction and 
assignment.

    #include "jsoncons/json.hpp"
    #include <boost/numeric/ublas/matrix.hpp>

    using jsoncons::json;
    using jsoncons::pretty_print;
    using boost::numeric::ublas::matrix;

This allows you, for example, to insert a boost matrix into a `json` object, 
and to retrieve it back cast to the appropriate type. You can do so by wrapping it in
a `json::any value`, like this:

    json obj;

    matrix<double> A(2,2);
    A(0,0) = 1;
    A(0,1) = 2;
    A(1,0) = 3;
    A(1,1) = 4;

    obj.set("mydata",json::any(A));

    matrix<double> B = obj["mydata"].as<json::any>().cast<matrix<double>>();

