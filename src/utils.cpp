#pragma once

#include <algorithm>
#include <thread>
#include <functional>
#include <string>
#include <vector>

using namespace std;

/*
 * Code extracted from
 * https://stackoverflow.com/questions/5888022/split-string-by-single-spaces
 */
size_t split(const string &txt, vector<string> &strs, char ch)
{
  size_t pos = txt.find(ch);
  size_t initialPos = 0;
  strs.clear();
  while (pos != string::npos)
  {
    strs.push_back(txt.substr(initialPos, pos - initialPos));
    initialPos = pos + 1;

    pos = txt.find(ch, initialPos);
  }
  strs.push_back(txt.substr(initialPos, min(pos, txt.size()) - initialPos + 1));
  return strs.size();
}

/*
 * Code extracted from
 * https://stackoverflow.com/a/18619293/9944075
 */
template <typename T>
const bool Contains(std::vector<T> &Vec, const T &Element)
{
  if (std::find(Vec.begin(), Vec.end(), Element) != Vec.end())
    return true;

  return false;
}

template <typename T>
vector<T> vec_stoi(const vector<string> vec)
{
  vector<T> output;
  for (auto it = vec.begin(); it != vec.end(); ++it)
    output.push_back((T)stoi(*it));
  return output;
}

/*
 * Code extracted from
 * https://stackoverflow.com/a/49188371/9944075
 */
/// @param[in] nb_elements : size of your for loop
/// @param[in] lock : lock for inserting stuff on results
/// @param[in] results : results vector
/// @param[in] functor(start, end, lock, results) :
/// your function processing a sub chunk of the for loop.
/// "start" is the first index to process (included) until the index "end"
/// (excluded)
/// @code
///     for(int i = start; i < end; ++i)
///         stuff = computation(i);
///     lock.lock();
///     results.push_back(stuff);
///     lock.unlock();
/// @endcode
/// @param use_threads : enable / disable threads.
///
///
template <typename T>
static void parallel_for(unsigned nb_elements,
                         mutex *lock,
                         vector<T> results,
                         std::function<void(int start, int end, mutex *lock, vector<T> results)> functor,
                         bool use_threads = true)
{
  // -------
  unsigned nb_threads_hint = std::thread::hardware_concurrency();
  unsigned nb_threads = nb_threads_hint == 0 ? 8 : (nb_threads_hint);

  unsigned batch_size = nb_elements / nb_threads;
  unsigned batch_remainder = nb_elements % nb_threads;

  std::vector<std::thread> my_threads(nb_threads);

  if (use_threads)
  {
    // Multithread execution
    for (unsigned i = 0; i < nb_threads; ++i)
    {
      int start = i * batch_size;
      my_threads[i] = std::thread(functor, start, start + batch_size, lock, results);
    }
  }
  else
  {
    // Single thread execution (for easy debugging)
    for (unsigned i = 0; i < nb_threads; ++i)
    {
      int start = i * batch_size;
      functor(start, start + batch_size, lock, results);
    }
  }

  // Deform the elements left
  int start = nb_threads * batch_size;
  functor(start, start + batch_remainder, lock, results);

  // Wait for the other thread to finish their task
  if (use_threads)
    std::for_each(my_threads.begin(), my_threads.end(), std::mem_fn(&std::thread::join));
}
