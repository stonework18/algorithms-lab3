#include <iostream>
#include <algorithm>
#include <stdexcept>
using namespace std;
template <typename T>
class Vector {
private:
	T* data;
	int sz;
	int cap;
public:
	// Default constructor
	Vector() : data(nullptr), sz(0), cap(0) {}

	//  Size constructor
	explicit Vector(int n) : sz(n), cap(n) {
		data = new T[cap];
		for (int i = 0; i < sz; i++) data[i] = T();
	}
	//  Copy constructor
	Vector(const Vector& other) : sz(other.sz), cap(other.cap) {
		data = new T[cap];
		for (int i = 0; i < sz; ++i) {
			data[i] = other.data[i];
		}
	}
	// Move constructor
	Vector(Vector&& other) noexcept :
		data(other.data), sz(other.sz), cap(other.cap) {
		other.data = nullptr;
		other.sz = 0;
		other.cap = 0;
	}
	// Destructor
	~Vector() { delete[] data; }

	void push_back(const T& value) {
		if (sz >= cap) {
			int newCap = (cap == 0) ? 1 : cap * 2;
			T* newData = new T[newCap];
			for (int i = 0; i < sz; i++) newData[i] = data[i];
			delete[] data;
			data = newData;
			cap = newCap;
		}
		data[sz++] = value;
	}

	// Operators
	Vector& operator=(Vector&& other) noexcept { // Move assignment operator
		if (this != &other) {
			delete[] data;
			data = other.data;
			sz = other.sz;
			cap = other.cap;
			other.data = nullptr;
			other.sz = 0;
			other.cap = 0;
		} return *this;
	}

	Vector& operator=(const Vector& other) { // Copy assignment operator
		if (this != &other) {
			Vector temp(other);
			swap(temp);
		} return *this;
	}
	// Iterators
	T* begin() { return data; }
	T* end() { return data + sz; }
	const T* begin() const { return data; }
	const T* end() const { return data + sz; }
	// Insertion iterator
	void insert(T* pos, const T& value) {
		int index = pos - data;
		if (index < 0 || index > sz) return;
		if (sz >= cap) {
			int newCap = (cap == 0) ? 1 : cap * 2;
			T* newData = new T[newCap];
			for (int i = 0; i < index; i++) newData[i] = data[i];
			newData[index] = value;
			for (int i = index; i < sz; i++) newData[i + 1] = data[i];
			delete[] data;
			data = newData;
			cap = newCap;
		} else {
			for (int i = sz; i > index; i--) data[i] = data[i - 1];
			data[index] = value;
		} sz++;
	}

	// Iterator for deletion
	void erase(T* pos) {
		int index = pos - data;
		if (index < 0 || index >= sz) return;
		for (int i = index; i < sz - 1; i++) data[i] = data[i + 1];
		sz--;
	}
	// Additional functionality
	template<typename F>
	void apply(F&& f) { for (int i = 0; i < sz; i++) f(data[i]); }
	template <typename F>
	void apply(F&& f) const { for (int i = 0; i < sz; ++i) f(static_cast<const T&>(data[i])); }
};

int main() {
	Vector<int> v;

	for (int i = 1; i <= 5; ++i) { v.push_back(i); }
	cout << "Vector: ";
	v.apply([](int x) { cout << x << ' '; });
	cout << endl;

	Vector<int> v2(v);
	cout << "The output of the copy constructor: ";
	for (int x : v2) { cout << x << ' '; };
	cout << endl;

	Vector<int> v3(move(v2));
	cout << "The output of the move constructor: ";
	for (int x : v3) { cout << x << ' '; };

	cout << "\nState of vector copy after move: ";
	for (int x : v2) { cout << x << ' '; };

	auto it = v.begin() + 2;
	v.insert(it, 99);
	cout << "\nAfter inserting 99 at position 2: ";
	for (int x : v) cout << x << " ";
	cout << endl;

	it = v.begin() + 3;
	v.erase(it);
	cout << "After removing the element at position 3: ";
	for (int x : v) cout << x << " ";
	cout << endl;

	v.apply([](int& x) { x *= 2; });
	cout << "After x *= 2: ";
	v.apply([](int x) { cout << x << ' '; });
	cout << '\n';

	return 0;
}