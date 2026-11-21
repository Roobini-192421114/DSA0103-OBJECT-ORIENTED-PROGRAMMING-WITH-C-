// Shows the real difference between `throw;` and `throw e;` when the catch parameter is a BASE type.
#include <iostream>
#include <stdexcept>
struct ClaimError : std::runtime_error { using std::runtime_error::runtime_error; };
struct RejectedClaim : ClaimError { using ClaimError::ClaimError; };   // more derived type

void thrower() { throw RejectedClaim("exceeds cover"); }
void viaRethrow() { try { thrower(); } catch (const ClaimError& e) { throw; } }       // same object
void viaCopy()    { try { thrower(); } catch (const ClaimError& e) { throw e; } }     // copy, sliced to ClaimError

int main()
{
    try { viaRethrow(); } catch (const RejectedClaim&) { std::cout << "throw;   -> still RejectedClaim\n"; }
                          catch (const ClaimError&)    { std::cout << "throw;   -> sliced to ClaimError\n"; }
    try { viaCopy(); }    catch (const RejectedClaim&) { std::cout << "throw e; -> still RejectedClaim\n"; }
                          catch (const ClaimError&)    { std::cout << "throw e; -> sliced to ClaimError\n"; }
}
