int main(void) {
    int* pT = reinterpret_cast<int*>(::operator new[](100));
    ::operator delete[](pT);
    return 0;
}
