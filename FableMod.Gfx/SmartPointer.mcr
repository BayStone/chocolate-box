// Insert in root class declaration.
#define GFX_DECLARE_ROOT_SMARTPOINTER \
    public: \
        void IncReferences() \
        { \
            ++m_uiReferences; \
        } \
        unsigned int DecReferences() \
        { \
			if (m_uiReferences > 0) \
				--m_uiReferences; \
			return m_uiReferences; \
        } \
        unsigned int GetReferenceCount() const \
        { \
            return m_uiReferences; \
        } \
    private: \
        unsigned int m_uiReferences

#define GFX_DECLARE_SMARTPOINTER(_classname) \
    class _classname; \
    typedef FableMod::Gfx::SmartPointer<_classname> _classname##Ptr; \
    typedef FableMod::Gfx::SmartPointer<_classname> _classname##Pointer
