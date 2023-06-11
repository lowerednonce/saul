#pragma once
#include <chrono>
#include "saul-base.hpp"

class Timer{
	public:
		Timer(){
			m_start = std::chrono::high_resolution_clock::now();
		}
		~Timer(){
			Stop();
		}
		void Stop(){
			m_end = std::chrono::high_resolution_clock::now();

			auto start = std::chrono::time_point_cast<std::chrono::microseconds>(m_start).time_since_epoch().count();
			auto end   =   std::chrono::time_point_cast<std::chrono::microseconds>(m_end).time_since_epoch().count();

			auto duration = end-start;
			float ms = duration * 0.001f;
			//TODO: export this data somehow, as this class will get destroyed uppon exiting the scope
		}
	private:
		std::chrono::time_point<std::chrono::high_resolution_clock> m_start;
};
