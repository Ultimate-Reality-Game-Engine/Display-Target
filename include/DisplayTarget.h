#ifndef ULTREALITY_RENDERING_DISPLAY_TARGET_H
#define ULTREALITY_RENDERING_DISPLAY_TARGET_H

#if defined(__GNUC__) or defined(__clang__)
#define FORCE_INLINE inline __attribute__((always_inline))
#elif defined(_MSC_VER)
#define FORCE_INLINE __forceinline
#else
#define FORCE_INLINE inline
#endif

namespace UltReality::Rendering
{
	/// <summary>
	/// A platform agnostic abstraction for windowing. Provides a mechanism for Renderers to obtain handles (pointers) to a window to target for rendering
	/// </summary>
	class DisplayTarget
	{
	protected:
		void* nativeHandle = nullptr; // Platform-specific pointer (HWND on Windows, )

	public:
		/// <summary>
		/// Create the structure and have it point to a window
		/// </summary>
		/// <param name="targetHandle">A pointer to the window this abstracts</param>
		DisplayTarget(void* targetHandle) noexcept : nativeHandle(targetHandle)
		{}

#if defined(_WIN_TARGET)
		/// <summary>
		/// Available on Windows Targets. Casts the internal pointer back to a HWND handle
		/// </summary>
		/// <returns></returns>
		FORCE_INLINE HWND ToHWND() const noexcept
		{
			return static_cast<HWND>(nativeHandle);
		}
#endif
	};
}

#endif //!ULTREALITY_RENDERING_DISPLAY_TARGET_H