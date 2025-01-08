// generica classe vector
class vector
{
    int sz;
    double *elem;

public:
    vector(int s) : sz{s}, elem{new double[s]}
    {
        for (int i = 0; i < s; i++)
            elem[i] = 0;
    }

    vector(initializer_list<double> lst) : sz{lst.size()}, elem{new double[sz]} // initializer_list<T> TEMPLATE
    {
        std::copy(lst.begin(), lst.end(), elem); // STANDARD COPY
    }

    // COSTRUTTORE DI COPIA
    vector(const vector &arg) : sz{arg.sz}, elem{new double[arg.sz]}
    {
        std::copy(arg.elem, arg.elem + sz, elem);
    }

    // ASSEGNAMENTO DI COPIA
    vector &operator=(const vector &a)
    {
        double *p = new double[a.sz];   // alloca nuovo spazio
        copy(a.elem, a.elem + a.sz, p); // copia gli elementi
        delete[] elem;                  // de-alloco vecchio di spazio
        elem = p;                       // resetto puntatore
        sz = a.sz;                      // resetto dimensione
        return *this;                   // ritorno self-reference
    }

    // COSTRUTTORE DI SPOSTAMENTO
    vector(vector &&a) : sz{a.sz}, elem{a.elem}
    {
        a.sz = 0;         // annulla a
        a.elem = nullptr; // invalida il puntatore di a
    }

    // ASSEGNAMENTO DI SPOSTAMENTO
    vector &operator=(vector &&a)
    {
        delete[] elem;    // dealloca lo spazio
        elem = a.elem;    // riassegna il puntatore
        sz = a.sz;        // riassegno il size
        a.elem = nullptr; // invalida il vecchio puntatore di a
        a.sz = 0;         // azzera il size di a
        return *this;     // ritorna una self-reference
    }

    // ACCESSO
    double &operator[](int n)           // lettura e scrittura di vettori non costanti
        double operator[](int n) const; // lettura di vettori costanti

    ~delete() { delete[] elem } // DISTRUTTORE
};

// TEMPLATE: permette di usare un tipo come parametro per una classe o una funzione
template <typename T>
class vector
{
    int sz T *elem;

public:
    vector() : sz{0}, elem { nullptr };

    explicit vector(int s) : sz{s}, elem{new T[s]}
    {
        for (int i = 0; i < sz; ++i)
            elem[i] = 0;
    }

    template <typename T> // VA SEMPRE RIPETUTO
    T myMax(T x, T y)     // esempio di FUNCTION TEMPLATE
    {
        return (x > y) ? x : y; // assunzione: esiste la comparazione tra elementi di tipo T
    }

    /* per chiamare la funzione:
    int x = i; j = 5;
    int max = myMax<int> (i, j);
    */
};

// INTERI COME PARAMETRI TEMPLATE
template <typename T, int N>
struct array
{
    T elem[N]; // dimensione fissa

    T &operator[](int n);
    const T &operator[](int n) const;

    T *data() { return elem; }
    const T *data() const { return elem; }

    int size() const { return N; }
};

// EREDITARIETA'

struct Point
{
    int x, int y;
};

class Shape // VERSIONE ASTRATTA
{
public:
    void draw() const;
    virtual void move(int dx, int dy) = 0; // VIRTUAL
    Point point(int i) const;
    int number_of_points() const;
    virtual ~Shape() {} // distruttore VIRTUAL

    // PER EVITARE DI INSERIRE UNA CLASSE DERIVATA IN UNO SLOT CONTENENTE MEMBRI DELLA CLASSE BASE
    Shape(const Shape &) = delete // disibilitato costruttore di copia
                           Shape
                           & operator=(const Shape &) = delete; // disabilitato operator=

protected: // essendo costruttori protected Shape non è implementabile
    Shape() {}
    Shape(initializer_list<Point> lst);

    virtual void draw_lines() const = 0; // VIRTUAL
    void add(Point p);

private:
    std::vector<Point> points;
};

class Circle : public Shape // circle eredita shape
{
    // questa classe può essere istanziata
    void draw_lines() const // override perché in Shape era virtual
}

template <typename Iterator> // Iterator è il nome, posso sceglierlo
Iterator high(Iterator first, Iterator last)
{
    Iterator high = first;
    for (Iterator p = first; p != last; ++p)
    {
        if (*high < *p)
            high = p
    }
    return high;
}

// LINKED LIST
template <typename Elem>
struct Link // nodo della linked list
{
    Link *prev;
    Link *succ;
    Elem val;
};

template <typename Elem>
struct list // classe di tipo Elem
{
    Link<Elem> *first;
    Link<Elem> *last;
}

template <typename Elem>
class List
{
public:
    class iterator; // forward declaration: da qualche parte c'è una classe chiamata iterator

    iterator begin();
    iterator end();

    iterator insert(iterator p, const Elem &v);
    iterator erase(iterator p);

    void push_back(const Elem *v);
    void push_front(const ELem *v);
    void pop_front();
    void pop_back();

    Elem &front();
    Elem &back();
};

template <typename Elem>
class list<Elem>::iterator
{
    Link<Elem> *curr;

public:
    iterator(Link<Elem> *p) : curr{p} {}
    iterator &operator++()
    {
        curr = curr->succ;
        return *this;
    }
    iterator &operator--()
    {
        curr = curr->prev;
        return *this;
    }
    Elem &operator*() { return curr->val: }
    bool operator==(const iterator &b) const { return curr == b.curr; }
    bool operator!=(const iterator &b) const { return curr != b.curr; }
};

template <typename T>
void user(vector<T> &v, list<T> &lst)
{
    for (/*vector<T>::iterator*/ auto p = v.begin(); p != v.end(); ++p)
    {
        cout << *p << "\n";
    }
    /*list<T>::iterator*/ auto q = find(lst.begin(), lst.end(), T{42});
}

// STD::FIND()

template <typename In, typename T> // In: iteratore di input, T: tipo elemento
In find(In first, In last, const T &val)
{
    while (first != last && *first != val)
        ++first;
    return first;
}

template <typename In, typename Pred>
In find_if(In first, In last, Pred pred)
{
    while (first != last && !pred(*first))
        ++first;
    return first;
}

// definire il predicato (esempio)
bool odd(int x)
{
    return x % 2;
}

// utilizzo
void f(vector<int> &v)
{
    auto p = std::find_if(v.begin(), v.end(), odd); // odd senza parentesi perché sto passando un puntatore a quella funzione
}

// FUNCTION OBJECT
class Larger_than
{
int v : public : Largen_than(int vv) : v{vv} {}

    bool operator()(int x) const { return x > v; } // override operator()
}

// pred(*first) viene tradotto in Larger_than::operator(*first)
// ora la funzione f diventa

void
f(std::list<double> &v, int x)
{
    auto p = std::find_if(v.begin(), v.end(), Larger_than(x));
}

// STD::SORT () ordina tra due iteratori

template <typename Ran>
void sort(Ran first, Ran last); // ordinamento con "<"

template <typename Ran, typename Cmp> // Cmp: funzione di ordinamento su Ran
void sort(Ran first, Ran last, Cmp cmp);

void test(std::vector<int> &v)
{
    std::sort(v.begin(), v.end());
}

// funzione sort() per i contenitori (diversa da std::sort())
template <typename C>
void sort(C &c)
{
    std::sort(c.begin(), c.end());
}

// STD::COPY()
template <typename In, typename Out>
Out copy(In first, In last, Out res)
{
    while (first != last)
    {
        *res = *first;
        ++res;
        ++first;
    }
    return res;
}

void f(std::vector<double> &vd, std::list<int> &li)
{
    if (vd.size() < li.size())
        error();
    std::copy(li.begin(), li.end(), vd.begin()); // vd.begin(): da dove parto a fare la copia
}

void f(const vector<int> &v)
{
    std::vector<int> v2(v.size());
    std::copy_if(v.begin(), v.end(), v2.begin(), Larger_than(6)); // larger_than: FUNCTION OBJECT
}

// STD::EXCEPTION
class exception
{
public:
    exception();

    exception(const exception &);

    exception &operator=(const exception &);

    virtual ~exception();

    virtual const char *what() const;
}

// derivare una classe dalla standard expceptions
class My_error : runtime_error
{
public:
    My_error(int x) : error_value{x} {}
    int error_value;

    const char *what() const override { return "My_error"; }
};

// UNIQUE_PTR SMART POINTER
template <typename T>
class unique_ptr
{
    T *_ptr = nullptr;

public:
    unique_ptr() = default;
    unique_ptr(T *ptr);

    ~unique_ptr();

    T operator*();
    T *operator->();

    T *release();

    unique_ptr(const unique_ptr<T> &ptr) = delete;
    unique_ptr &operator=(const unique_ptr<T> &ptr) = delete;
}

template <typename T>
unique_ptr<T>::unique_ptr(T *ptr)
{
    _ptr = ptr;
}

template <typename T>
T unique_ptr<T>::operator*()
{
    return *_ptr;
}

template <typename T>
unique_ptr<T>::~unique_ptr()
{
    delete _ptr;
    _ptr = nullptr;
}

template <typename T>
T *unique_ptr<T>::release()
{
    T *ptr = _ptr
        _ptr = nullptr;
    return ptr;
}

template <typename T>

T *unique_ptr<T>::operator->()
{
    return _ptr;
}

std::vector<int> *make_vec()
{
    std::unique_ptr<std::vector<int>> p{new std::vector<int>};
    return p.release();
}

// unique_ptr SOLO SPOSTATO, NON COPIATO
std::unique_ptr<int> AllocateAndReturn()
{
    std::unique_ptr<int> ptr_to_return(new int(42));
    return ptr_to_return;
}

// con shared_ptr è possibile la copia
std::shared_ptr<int> AllocateAndReturn()
{
    std::shared_ptr<int> ptr_to_return(new int(42));
    return ptr_to_return;
}

int main()
{
    std::shared_ptr<int> moved_ptr = AllocateAndReturn();

    std::shared_ptr<int> copied_ptr = moved_ptr;

    std::cout << *moved_ptr << std::endl;

    return 0;
}