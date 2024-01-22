#pragma once
#include <iterator>
#include <memory>
#include <stdexcept>

namespace fefu_laboratory_two
{
	template <typename T>
	class Allocator {
	public:
		using value_type = T;
		using size_type = std::size_t;
		using pointer = T*;

		Allocator() noexcept = default;

		Allocator(const Allocator& other) noexcept {};

		template <class U>
		Allocator(const Allocator<U>& other) noexcept {};

		~Allocator() = default;

		pointer allocate(size_type n) {
			pointer ptr = static_cast<pointer>(malloc(sizeof(value_type) * n));
			if (ptr) return ptr;
			throw std::bad_alloc();
		};

		void deallocate(pointer p) noexcept { free(p); };
	};

	template <typename ValueType>
	class IChunkList {
	public:
		virtual ~IChunkList() = default;
		virtual size_t size() const noexcept = 0;
		virtual ValueType& at(size_t pos) = 0;
		virtual ValueType& operator[](size_t n) = 0;
	};

	template <typename value_type>
	class Iterator {
	protected:
		IChunkList<value_type>* list = nullptr;
		value_type* _current_value = nullptr;
		int _index = 0;
	public:
		using iterator_category = std::random_access_iterator_tag;
		using difference_type = std::ptrdiff_t;
		using pointer = value_type*;
		using reference = value_type&;

		virtual int index() { return _index; };

		constexpr Iterator() noexcept = default;

		Iterator(IChunkList<value_type>* chunk, int index, value_type* value) :
			list(chunk),
			_index(index),
			_current_value(value)
		{};

		Iterator(const Iterator& other) noexcept = default;

		Iterator& operator=(const Iterator&) = default;

		virtual ~Iterator() = default;

		friend void swap(Iterator<value_type>& a, Iterator<value_type>& b) {
			std::swap(a.list, b.list);
			std::swap(a._current_value, b._current_value);
			std::swap(a._index, b._index);
		};

		friend bool operator==(const Iterator<value_type>& lhs,
			const Iterator<value_type>& rhs) {
			return lhs._current_value == rhs._current_value;
		};
		friend bool operator!=(const Iterator<value_type>& lhs,
			const Iterator<value_type>& rhs) {
			return !(lhs._current_value == rhs._current_value);
		};

		reference operator*() const { return *_current_value; };
		pointer operator->() const { return _current_value; };

		Iterator operator++(int) {
			if (_index + 1 == list->size())
				return Iterator();
			_index++;
			_current_value = &list->at(_index);
			return *this;
		};
		Iterator operator--(int) {
			_index--;
			if (_index <= -1)
				return *this;

			_current_value = &list->at(_index);
			return *this;
		};

		Iterator& operator++() {
			if (_index + 1 == list->size()) {
				this->_current_value = nullptr;
				this->list = nullptr;
				this->_index = 0;
				return *this;
			}
			_index++;
			_current_value = &list->at(_index);
			return *this;
		};

		Iterator& operator--() {
			_index--;
			if (_index <= -1)
				return *this;

			_current_value = &list->at(_index);
			return *this;
		};

		Iterator operator+(const difference_type& n) const {
			return Iterator(list, _index + n, &list->at(_index + n));
		};


		Iterator operator-(const difference_type& n) const {
			return Iterator(list, _index - n, &list->at(_index - n));
		};

		Iterator& operator+=(const difference_type& n) {
			_index += n;
			_current_value = &list->at(_index);
			return *this;
		};

		Iterator& operator-=(const difference_type& n) {
			_index -= n;
			_current_value = &list->at(_index);
			return *this;
		};

		reference operator[](const difference_type& n) {
			return list[n];
		};

		friend bool operator<(const Iterator<value_type>& lhs,
			const Iterator<value_type>& rhs) {
			return rhs._index - lhs._index > 0;
		};
		friend bool operator<=(const Iterator<value_type>& lhs,
			const Iterator<value_type>& rhs) {
			return !(rhs._index > lhs._index);
		};
		friend bool operator>(const Iterator<value_type>& lhs,
			const Iterator<value_type>& rhs) {
			return rhs._index < lhs._index;
		};
		friend bool operator>=(const Iterator<value_type>& lhs,
			const Iterator<value_type>& rhs) {
			return !(lhs._index < rhs._index);
		};
	};

	template <typename value_type>
	class ConstIterator : public Iterator<value_type> {
	public:
		using iterator_category = std::random_access_iterator_tag;
		using difference_type = std::ptrdiff_t;
		using const_pointer = const value_type*;
		using const_reference = const value_type&;
		constexpr ConstIterator() : Iterator<value_type>() {};
		ConstIterator(IChunkList<value_type>* chunk, int index, value_type* value) :
			Iterator<value_type>(chunk, index, value) {};

		ConstIterator(const IChunkList<value_type>* chunk, int index, const value_type* value) :
			Iterator<value_type>(const_cast<IChunkList<value_type>*>(chunk), index, const_cast<value_type*>(value)) {};

		ConstIterator(const ConstIterator& other) noexcept = default;

		ConstIterator& operator=(const ConstIterator&) = default;

		~ConstIterator() override = default;

		friend void swap(ConstIterator<value_type>& a, ConstIterator<value_type>& b) {
			std::swap(a.list, b.list);
			std::swap(a._current_value, b._current_value);
			std::swap(a._index, b._index);
		};

		friend bool operator==(const ConstIterator<value_type>& lhs,
			const ConstIterator<value_type>& rhs) {
			return lhs._current_value == rhs._current_value;
		};
		friend bool operator!=(const ConstIterator<value_type>& lhs,
			const ConstIterator<value_type>& rhs) {
			return !(lhs._current_value == rhs._current_value);
		};

		const_reference operator*() const { return *this->_current_value; };
		const_pointer operator->() const { return this->_current_value; };
		const_reference operator[](const difference_type& n) {
			return this->list[n];
		};

		ConstIterator operator++(int) {
			if (this->_index + 1 == this->list->size())
				return ConstIterator();
			this->_index++;
			this->_current_value = &this->list->at(this->_index);
			return *this;
		};

		ConstIterator operator--(int) {
			return Iterator<value_type>::operator--(0);
		};

		ConstIterator& operator++() {
			if (this->_index + 1 == this->list->size()) {
				this->_current_value = nullptr;
				this->list = nullptr;
				this->_index = 0;
				return *this;
			}
			this->_index++;
			this->_current_value = &this->list->at(this->_index);
			return *this;
		};

		ConstIterator& operator--() {
			if (this->_index - 1 == -1)
				throw std::exception();
			this->_index--;
			this->_current_value = &this->list->at(this->_index);
			return *this;
		};

		ConstIterator operator+(const difference_type& n) const {
			return ConstIterator(this->list, this->_index + n, &this->list->at(this->_index + n));
		};


		ConstIterator operator-(const difference_type& n) const {
			return ConstIterator(this->list, this->_index - n, &this->list->at(this->_index - n));
		};

		ConstIterator& operator+=(const difference_type& n) {
			this->_index += n;
			this->_current_value = &this->list->at(this->_index);
			return *this;
		};

		ConstIterator& operator-=(const difference_type& n) {
			this->_index -= n;
			this->_current_value = &this->list->at(this->_index);
			return *this;
		};

		friend bool operator<(const ConstIterator<value_type>& lhs,
			const ConstIterator<value_type>& rhs) {
			return rhs._index - lhs._index > 0;
		};

		friend bool operator<=(const ConstIterator<value_type>& lhs,
			const ConstIterator<value_type>& rhs) {
			return !(rhs._index > lhs._index);
		};

		friend bool operator>(const ConstIterator<value_type>& lhs,
			const ConstIterator<value_type>& rhs) {
			return rhs._index < lhs._index;
		};

		friend bool operator>=(const ConstIterator<value_type>& lhs,
			const ConstIterator<value_type>& rhs) {
			return !(lhs._index < rhs._index);
		};
	};

	template <typename T, int N, typename Allocator = Allocator<T>>
	class ChunkList : public IChunkList<T> {
		class ChunkNode {
		public:
			T* list = nullptr;
			ChunkNode* prev = nullptr;
			ChunkNode* next = nullptr;
			int node_size = 0;
			Allocator allocator;
			ChunkNode(const Allocator& alloc = Allocator()) : allocator(alloc) {
				list = allocator.allocate(N);
			}

			ChunkNode(ChunkNode *other)
			{
				list = allocator.allocate(N);
				for (size_type i = 0; i < N; i++)
				{
					list[i] = other->list[i];
				}
				node_size = other->node_size;
			}
		};
	protected:
		ChunkNode* first = nullptr;
		int list_size = 0;
	public:

		using value_type = T;
		using allocator_type = Allocator;
		using size_type = std::size_t;
		using difference_type = std::ptrdiff_t;
		using reference = value_type&;
		using const_reference = const value_type&;
		using pointer = value_type*;
		using const_pointer = const pointer;
		using iterator = Iterator<value_type>;
		using const_iterator = ConstIterator<value_type>;

		/// @brief Default constructor. Constructs an empty container with a
		/// default-constructed allocator.
		ChunkList() : first(new ChunkNode()) {};

		/// @brief Constructs an empty container with the given allocator
		/// @param alloc allocator to use for all memory allocations of this container
		/// s
		/*explicit ChunkList(const Allocator& alloc);*/

		/// @brief Constructs the container with count copies of elements with value
		/// and with the given allocator
		/// @param count the size of the container
		/// @param value the value to initialize elements of the container with
		/// @param alloc allocator to use for all memory allocations of this container
		ChunkList(size_type count, const T& value = T(), const Allocator& alloc = Allocator())
			: first(new ChunkNode())
		{
			for (size_type i = 0; i < count; i++)
				push_back(value);
		};

		/// @brief Constructs the container with count default-inserted instances of
		/// T. No copies are made.
		/// @param count the size of the container
		/// @param alloc allocator to use for all memory allocations of this container
		explicit ChunkList(size_type count, const Allocator& alloc = Allocator())
			: first(new ChunkNode())
		{
			for (size_type i = 0; i < count; i++) push_back(value_type());
		};

		/// @brief Constructs the container with the contents of the range [first,
		/// last).
		/// @tparam InputIt Input Iterator
		/// @param first, last 	the range to copy the elements from
		/// @param alloc allocator to use for all memory allocations of this container
		template <class InputIt>
		ChunkList(InputIt first, InputIt last, const Allocator& alloc = Allocator()) {
			if (first == last) return;
			this->first = new ChunkNode(alloc);

			auto it = first;

			for (; it != last; ++it) push_back(*it);
		};

		/// @brief Copy constructor. Constructs the container with the copy of the
		/// contents of other.
		/// @param other another container to be used as source to initialize the
		/// elements of the container with
		ChunkList(const ChunkList& other) {

			ChunkNode* otherNode = other.first;
			if (otherNode == nullptr) return;
			first = new ChunkNode(otherNode);
			ChunkNode* node = first;

			while (otherNode->next != nullptr)
			{
				node->next = new ChunkNode(otherNode->next);
				node = node->next;
				otherNode = otherNode->next;
			}

			list_size = other.list_size;
		};

		/// @brief Constructs the container with the copy of the contents of other,
		/// using alloc as the allocator.
		/// @param other another container to be used as source to initialize the
		/// elements of the container with
		/// @param alloc allocator to use for all memory allocations of this container
		ChunkList(const ChunkList& other, const Allocator& alloc) {
			this = ChunkList(other);
			ChunkNode* tmp = this->first;
			while (tmp != nullptr) {
				tmp->allocator = alloc;
				tmp = tmp->next;
			}
		};

		/**
		 * @brief Move constructor.
		 *
		 * Constructs the container with the contents of other using move semantics.
		 * Allocator is obtained by move-construction from the allocator belonging to
		 * other.
		 *
		 * @param other another container to be used as source to initialize the
		 * elements of the container with
		 */
		ChunkList(ChunkList&& other) {
			first = std::move(other.first);
			list_size = std::move(other.list_size);
			other.clear();
		};

		/**
		 * @brief Allocator-extended move constructor.
		 * Using alloc as the allocator for the new container, moving the contents
		 * from other; if alloc != other.get_allocator(), this results in an
		 * element-wise move.
		 *
		 * @param other another container to be used as source to initialize the
		 * elements of the container with
		 * @param alloc allocator to use for all memory allocations of this container
		 */
		ChunkList(ChunkList&& other, const Allocator& alloc) {
			this = ChunkList(other);
			ChunkNode* tmp = first;
			while (tmp != nullptr) {
				tmp->allocator = alloc;
				tmp = tmp->next;
			}
		};

		/// @brief Constructs the container with the contents of the initializer list
		/// init.
		/// @param init initializer list to initialize the elements of the container
		/// with
		/// @param alloc allocator to use for all memory allocations of this container
		ChunkList(std::initializer_list<T> init, const Allocator& alloc = Allocator()) {
			if (init.size() == 0) return;
			first = new ChunkNode(alloc);

			auto it = init.begin();

			for (; it != init.end(); ++it)
				push_back(*it);
		};

		/// @brief Destructs the ChunkList.
		~ChunkList() override {
			clear();
		};

		/// @brief Copy assignment operator. Replaces the contents with a copy of the
		/// contents of other.
		/// @param other another container to use as data source
		/// @return *this
		ChunkList& operator=(const ChunkList& other) {
			(this) = new ChunkList(other);
			return (*this);
		};

		/**
		 * Move assignment operator.
		 *
		 * Replaces the contents with those of other using move semantics
		 * (i.e. the data in other is moved from other into this container).
		 * other is in a valid but unspecified state afterwards.
		 *
		 * @param other another container to use as data source
		 * @return *this
		 */
		ChunkList& operator=(ChunkList&& other) {
			if (this == &other)
				return this;
			first = std::move(other.first);
			list_size = std::move(other.list_size);
			other.clear();

			return *this;
		};

		/// @brief Replaces the contents with those identified by initializer list
		/// ilist.
		/// @param ilist
		/// @return this
		ChunkList& operator=(std::initializer_list<T> ilist) {
			auto it = ilist.begin();

			for (; it != ilist.end(); ++it) push_back(*it);
			return *this;
		};

		/// @brief Replaces the contents with count copies of value
		/// @param count
		/// @param value
		void assign(size_type count, const T& value) {
			clear();
			for (size_type i = 0; i < count; i++) push_back(value);
			list_size = count;
		};

		/// @brief Replaces the contents with copies of those in the range [first,
		/// last).
		/// @tparam InputIt
		/// @param first
		/// @param last
		template <class InputIt>
		void assignIt(InputIt first, InputIt last) {
			if (first == last) return;
			clear();

			auto it = first;

			for (; it != last; ++it) push_back(*it);
		};

		/// @brief Replaces the contents with the elements from the initializer list
		/// ilis
		/// @param ilist
		void assign(std::initializer_list<T> ilist) {
			if (ilist.size() == 0) return;
			clear();

			auto it = ilist.begin();

			for (; it != ilist.end(); ++it) push_back(*it);
		};

		/// @brief Returns the allocator associated with the container.
		/// @return The associated allocator.
		allocator_type get_allocator() const noexcept {
			return first->allocator;
		};

		ChunkNode* last_chunk() {
			ChunkNode* tmp = first;
			while (tmp->next != nullptr)
				tmp = tmp->next;
			return tmp;
		}

		/// ELEMENT ACCESS

		/// @brief Returns a reference to the element at specified location pos, with
		/// bounds checking. If pos is not within the range of the container, an
		/// exception of type std::out_of_range is thrown.
		/// @param pos position of the element to return
		/// @return Reference to the requested element.
		/// @throw std::out_of_range
		reference at(size_type pos) override {
			int chunk_index = pos / N;
			int elemnt_index = pos % N;
			if (pos >= max_size() || pos < 0) throw std::out_of_range("Out of range");

			ChunkNode* tmp = first;
			while (chunk_index > 0) {
				tmp = tmp->next;
				chunk_index--;
			}
			return tmp->list[elemnt_index];
		};

		/// @brief Returns a const reference to the element at specified location pos,
		/// with bounds checking. If pos is not within the range of the container, an
		/// exception of type std::out_of_range is thrown.
		/// @param pos position of the element to return
		/// @return Const Reference to the requested element.
		/// @throw std::out_of_range
		const_reference at(size_type pos) const {
			int chunk_index = pos / N;
			int elemnt_index = pos % N;
			if (pos >= max_size() || pos < 0)
				throw std::out_of_range("Out of range");

			ChunkNode* tmp = first;
			while (chunk_index > 0) {
				tmp = tmp->next;
				chunk_index--;
			}
			return tmp->list[elemnt_index];
		};

		/// @brief Returns a reference to the element at specified location pos. No
		/// bounds checking is performed.
		/// @param pos position of the element to return
		/// @return Reference to the requested element.
		reference operator[](size_type pos) override {
			return at(pos);
		};

		/// @brief Returns a const reference to the element at specified location pos.
		/// No bounds checking is performed.
		/// @param pos position of the element to return
		/// @return Const Reference to the requested element.
		const_reference operator[](size_type pos) const {
			return at(pos);
		};

		/// @brief Returns a reference to the first element in the container.
		/// Calling front on an empty container is undefined.
		/// @return Reference to the first element
		reference front() {
			if (!list_size) throw std::logic_error("Empty container");

			return first->list[0];
		};

		/// @brief Returns a const reference to the first element in the container.
		/// Calling front on an empty container is undefined.
		/// @return Const reference to the first element
		const_reference front() const {
			if (!list_size) throw std::logic_error("Empty container");
			return first->list[0];
		};

		/// @brief Returns a reference to the last element in the container.
		/// Calling back on an empty container causes undefined behavior.
		/// @return Reference to the last element.
		reference back() {
			if (!list_size) throw std::logic_error("Empty container");

			ChunkNode* tmp = last_chunk();
			while (tmp->next != nullptr) tmp = tmp->next;

			return tmp->list[tmp->node_size - 1];
		};

		/// @brief Returns a const reference to the last element in the container.
		/// Calling back on an empty container causes undefined behavior.
		/// @return Const Reference to the last element.
		const_reference back() const {
			if (!list_size) throw std::logic_error("Empty");

			ChunkNode* tmp = last_chunk();
			return tmp->list[tmp->node_size - 1];
		};

		/// ITERATORS

		/// @brief Returns an iterator to the first element of the ChunkList.
		/// If the ChunkList is empty, the returned iterator will be equal to end().
		/// @return Iterator to the first element.
		iterator begin() noexcept { return Iterator<T>(this, 0, &at(0)); };

		/// @brief Returns an iterator to the first element of the ChunkList.
		/// If the ChunkList is empty, the returned iterator will be equal to end().
		/// @return Iterator to the first element.
		const_iterator begin() const noexcept { return ConstIterator<T>(this, 0, &at(0)); };

		/// @brief Same to begin()
		const_iterator cbegin() const noexcept { return begin(); };

		/// @brief Returns an iterator to the element following the last element of
		/// the ChunkList. This element acts as a placeholder; attempting to access it
		/// results in undefined behavior.
		/// @return Iterator to the element following the last element.
		iterator end() noexcept { return Iterator<T>(); };

		/// @brief Returns an constant iterator to the element following the last
		/// element of the ChunkList. This element acts as a placeholder; attempting to
		/// access it results in undefined behavior.
		/// @return Constant Iterator to the element following the last element.
		const_iterator end() const noexcept { return ConstIterator<T>(); };

		/// @brief Same to end()
		const_iterator cend() const noexcept { return end(); };

		/// CAPACITY

		/// @brief Checks if the container has no elements
		/// @return true if the container is empty, false otherwise
		bool empty() const noexcept { return !list_size; };

		/// @brief Returns the number of elements in the container
		/// @return The number of elements in the container.
		size_type size() const noexcept override { return list_size; };

		/// @brief Returns the maximum number of elements the container is able to
		/// hold due to system or library implementation limitations
		/// @return Maximum number of elements.
		size_type max_size() const noexcept {
			int r = list_size % N;
			return (r == 0 ? list_size : list_size + N - r);
		};

		/// @brief Requests the removal of unused capacity.
		/// It is a non-binding request to reduce the memory usage without changing
		/// the size of the sequence. All iterators and references are invalidated.
		/// Past-the-end iterator is also invalidated.
		void shrink_to_fit() {
			ChunkNode* tmp = last_chunk();
			while (tmp->node_size == 0)
			{
				tmp = tmp->prev;
				tmp->next = nullptr;
			}
		};

		/// MODIFIERS

		/// @brief Erases all elements from the container.
		/// nvalidates any references, pointers, or iterators referring to contained
		/// elements. Any past-the-end iterators are also invalidated.
		void clear() noexcept {
			ChunkNode* cur = first;
			while (cur != nullptr) {
				ChunkNode* tmp = cur;
				cur = cur->next;
				delete tmp;
			}
			list_size = 0;
			first = nullptr;
		};

		/// @brief Shifts all elements from pos to end to the right, increases size to fit all
		void shift_right(const_iterator pos) 
		{
			push_back(value_type());
			iterator it = iterator(this, list_size-1, &at(list_size - 1));

			while (true)
			{
				*it = *(it - 1);
				it--;
				if (it == pos) return;
			}
		}

		void shift_left(const_iterator pos)
		{
			iterator it = iterator(this, pos.index(), &at(pos.index()));
			while (true)
			{
				*it = *(it + 1);
				it++;
				if (it.index() == list_size - 1) break;
			}
			list_size--;
		}

		/// @brief Inserts value before pos.
		/// @param pos iterator before which the content will be inserted.
		/// @param value element value to insert
		/// @return Iterator pointing to the inserted value.
		iterator insert(const_iterator pos, const value_type& value) {
			shift_right(pos);
			iterator it = iterator(this, pos.index(), &at(pos.index()));
			(*it) = value;
			return it;
		};

		/// @brief Inserts value before pos.
		/// @param pos iterator before which the content will be inserted.
		/// @param value element value to insert
		/// @return Iterator pointing to the inserted value.
		iterator insert(const_iterator pos, T&& value) {
			shift_right(pos);
			iterator it = iterator(this, pos.index(), &at(pos.index()));
			(*it) = std::move(value);
			return it;
		};

		/// @brief Inserts count copies of the value before pos.
		/// @param pos iterator before which the content will be inserted.
		/// @param count number of elements to insert
		/// @param value element value to insert
		/// @return Iterator pointing to the first element inserted, or pos if count
		/// == 0.
		iterator insert(const_iterator pos, size_type count, const T& value)
		{
			if (count == 0) return pos;

			int index = pos.index();

			for (int i = 0; i < count; i++) insert(pos, value);

			return Iterator<T>(this, index, at(index));
		};

		/// @brief Inserts elements from range [first, last) before pos.
		/// @tparam InputIt Input Iterator
		/// @param pos iterator before which the content will be inserted.
		/// @param first,last the range of elements to insert, can't be iterators into
		/// container for which insert is called
		/// @return Iterator pointing to the first element inserted, or pos if first
		/// == last.
		template <class InputIt>
		iterator insert(const_iterator pos, InputIt first, InputIt last) {
			if (first == last) return pos;

			int index = pos.index();
			auto it = last - 1;
			while (true)
			{
				insert(pos, *it);
				if (it == first) break;
				it--;
			}

			return Iterator<T>(this, index, &at(index));
		};

		/// @brief Inserts elements from initializer list before pos.
		/// @param pos iterator before which the content will be inserted.
		/// @param ilist initializer list to insert the values from
		/// @return Iterator pointing to the first element inserted, or pos if ilist
		/// is empty.
		iterator insert(const_iterator pos, std::initializer_list<T> ilist)
		{
			if (ilist.size() == 0) return pos;
			int index = pos.index();
			auto it = ilist.end() - 1;

			while (true)
			{
				insert(pos, *it);
				if (it == ilist.begin()) break;
				it--;
			}

			return Iterator<T>(this, index, &at(index));
		};


		/// @brief Inserts a new element into the container directly before pos.
		/// @param pos iterator before which the new element will be constructed
		/// @param ...args arguments to forward to the constructor of the element
		/// @return terator pointing to the emplaced element.
		template <class... Args>
		iterator emplace(const_iterator pos, Args&&... args) {
			auto data = { args... };
			return insert(pos, data);
		};

		/// @brief Removes the element at pos.
		/// @param pos iterator to the element to remove
		/// @return Iterator following the last removed element.
		iterator erase(const_iterator pos) {
			size_type index = pos.index();
			shift_left(pos);
			return Iterator<T>(this, index, &at(index));
		};

		/// @brief Removes the elements in the range [first, last).
		/// @param first,last range of elements to remove
		/// @return Iterator following the last removed element.
		iterator erase(const_iterator first, const_iterator last) {
			size_type diff = last.index() - first.index();

			for (size_type i = 0; i < diff; i++)
				erase(first + 1);

			return Iterator<T>(this, first.index(), &at(first.index()));
		};

		/// @brief Appends the given element value to the end of the container.
		/// The new element is initialized as a copy of value.
		/// @param value the value of the element to append
		void push_back(const T& value) {
			if (first == nullptr)
				first = new ChunkNode();

			ChunkNode* tmp = last_chunk();
			if (tmp->node_size == N)
			{
				tmp->next = new ChunkNode();
				auto tmpPrev = tmp;
				tmp = tmp->next;
				tmp->prev = tmpPrev;
			}
			tmp->list[tmp->node_size] = value;
			tmp->node_size++;
			list_size++;
		};

		/// @brief Appends the given element value to the end of the container.
		/// Value is moved into the new element.
		/// @param value the value of the element to append
		void push_back(T&& value) {
			if (first == nullptr)
				first = new ChunkNode();

			ChunkNode* tmp = last_chunk();
			if (tmp->node_size == N)
			{
				tmp->next = new ChunkNode();
				tmp = tmp->next;
			}
			tmp->list[tmp->node_size] = std::move(value);
			tmp->node_size++;
			list_size++;
		};

		/// @brief Appends a new element to the end of the container.
		/// @param ...args arguments to forward to the constructor of the element
		/// @return A reference to the inserted element.
		template <class... Args>
		reference emplace_back(Args&&... args) {
			auto data = { args... };
			auto it = data.begin();
			for (; it != data.end(); it++)
				push_back(*it);
			return back();
		};

		/// @brief Removes the last element of the container.
		void pop_back() {
			this->list_size--;
			ChunkNode* tmp = last_chunk();
			tmp->node_size--;
		};

		/// @brief Prepends the given element value to the beginning of the container.
		/// @param value the value of the element to prepend
		void push_front(const T& value) {
			insert(cbegin(), value);
		};

		/// @brief Prepends the given element value to the beginning of the container.
		/// @param value moved value of the element to prepend
		void push_front(T&& value) {
			insert(cbegin(), std::move(value));
		};

		/// @brief Inserts a new element to the beginning of the container.
		/// @param ...args arguments to forward to the constructor of the element
		/// @return A reference to the inserted element.
		template <class... Args>
		reference emplace_front(Args&&... args) {
			auto data = { args... };
			auto it = data.end() - 1;
			while (true)
			{
				insert(cbegin(), *it);
				if (it == data.begin()) break;
				it--;
			}
			return back();
		};

		/// @brief Removes the first element of the container.
		void pop_front() {
			erase(cbegin());
		};

		/// @brief Resizes the container to contain count elements.
		/// If the current size is greater than count, the container is reduced to its
		/// first count elements. If the current size is less than count, additional
		/// default-inserted elements are appended
		/// @param count new size of the container
		void resize(size_type count) {
			if (count == list_size) return;
			if (count < list_size)
				while (list_size != count)
					pop_back();
			else
				while (list_size != count)
					push_back(T());
		};

		/// @brief Resizes the container to contain count elements.
		/// If the current size is greater than count, the container is reduced to its
		/// first count elements. If the current size is less than count, additional
		/// copies of value are appended.
		/// @param count new size of the container
		/// @param value the value to initialize the new elements with
		void resize(size_type count, const value_type& value) {
			if (count == list_size) return;
			if (count < list_size)
				while (list_size != count)
					pop_back();
			else
				while (list_size != count)
					push_back(value);
		};

		/// @brief Exchanges the contents of the container with those of other.
		/// Does not invoke any move, copy, or swap operations on individual elements.
		/// All iterators and references remain valid. The past-the-end iterator is
		/// invalidated.
		/// @param other container to exchange the contents with
		void swap(ChunkList& other) {
			ChunkNode* first_tmp;
			int size_tmp;

			first_tmp = other.first;
			size_tmp = other.list_size;

			other.first = first;
			other.list_size = list_size;

			first = first_tmp;
			list_size = size_tmp;
		};

		/// COMPARISIONS

		/// @brief Checks if the contents of lhs and rhs are equal
		/// @param lhs,rhs ChunkLists whose contents to compare
		friend bool operator==(const ChunkList& lhs,
			const ChunkList& rhs) {
			if (lhs.list_size != rhs.list_size)
				return false;

			for (int i = 0; i < lhs.list_size; i++)
				if (lhs.at(i) != rhs.at(i))
					return false;

			return true;
		};

		/// @brief Checks if the contents of lhs and rhs are not equal
		/// @param lhs,rhs ChunkLists whose contents to compare
		template <class U, int N, class Alloc>
		friend bool operator!=(const ChunkList& lhs,
			const ChunkList& rhs) {
			return !operator==(lhs, rhs);
		};

		/// @brief Compares the contents of lhs and rhs lexicographically.
		/// @param lhs,rhs ChunkLists whose contents to compare
		friend bool operator>(const ChunkList& lhs, const ChunkList& rhs) {
			
			if (lhs.list_size < rhs.list_size) return false;

			if (lhs.list_size > rhs.list_size) return true;

			for (int i = 0; i < lhs.list_size; i++)
				if (rhs.at(i) < lhs.at(i))
					return true;

			return false;
		};

		/// @brief Compares the contents of lhs and rhs lexicographically.
		/// @param lhs,rhs ChunkLists whose contents to compare
		friend bool operator<(const ChunkList& lhs, const ChunkList& rhs) {
			if ((lhs > rhs) || (lhs == rhs)) return false;
			return true;
		};

		/// @brief Compares the contents of lhs and rhs lexicographically.
		/// @param lhs,rhs ChunkLists whose contents to compare
		friend bool operator>=(const ChunkList& lhs,
			const ChunkList& rhs) {
			return !operator<(lhs, rhs);
		};

		/// @brief Compares the contents of lhs and rhs lexicographically.
		/// @param lhs,rhs ChunkLists whose contents to compare
		friend bool operator<=(const ChunkList& lhs,
			const ChunkList& rhs) {
			return !operator>(lhs, rhs);
		};
	};

	/// NON-MEMBER FUNCTIONS

	/// @brief  Swaps the contents of lhs and rhs.
	/// @param lhs,rhs containers whose contents to swap
	template <class T, int N, class Alloc>
	void swap(ChunkList<T, N, Alloc>& lhs, ChunkList<T, N, Alloc>& rhs) {
		std::swap(lhs, rhs);
	};

	/// @brief Erases all elements that compare equal to value from the container.
	/// @param c container from which to erase
	/// @param value value to be removed
	/// @return The number of erased elements.
	template <class T, int N, class Alloc, class U>
	typename ChunkList<T, N, Alloc>::size_type erase(ChunkList<T, N, Alloc>& c, const U& value);

	/// @brief Erases all elements that compare equal to value from the container.
	/// @param c container from which to erase
	/// @param pred unary predicate which returns ​true if the element should be
	/// erased.
	/// @return The number of erased elements.
	template <class T, int N, class Alloc, class Pred>
	typename ChunkList<T, N, Alloc>::size_type erase_if(ChunkList<T, N, Alloc>& c, Pred pred);
}