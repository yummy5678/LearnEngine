#pragma once

// コピー禁止クラス
class NonCopyable
{
public:
	NonCopyable() = default;
	~NonCopyable() = default;

	// コピーコンストラクタとコピー代入演算子を削除
	NonCopyable(const NonCopyable&) = delete;
	NonCopyable& operator=(const NonCopyable&) = delete;

	// ムーブは許可
	NonCopyable(NonCopyable&&) noexcept = default;
	NonCopyable& operator=(NonCopyable&&) noexcept = default;

};

