// MIT License
// 
// Copyright (c) 2018 Oscar B. et al.
// 
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:
// 
// The above copyright notice and this permission notice shall be included in all
// copies or substantial portions of the Software.
// 
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
// SOFTWARE.
#ifndef UTILS_H
#define UTILS_H

#include <set>


namespace gravit {
	class Observer;

	/**
	 * @brief A Subject base class as defined in the `Observer' pattern of the
	 * GOF book `Design Patterns'.
	 *
	 * TO-DO Document more, according to Doxygen.
	 */
	class Observable {
		private:
			std::set<Observer*> watchers {};
		public:
			virtual ~Observable();

			void virtual notify() const final;
			void virtual attach(Observer *o) final;
			void virtual detach(Observer *o) final;
	};

	/**
	 * TO-DO Document.
	 */
	class Observer {
		public:
			virtual ~Observer();
			void virtual onChange(Observable const *o) = 0;
	};
}


#endif
