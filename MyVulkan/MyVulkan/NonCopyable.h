#pragma once

// コピー禁止クラス
class NonCopyable
{
protected:
	NonCopyable() = default;
	~NonCopyable() = default;

	// コピーコンストラクタとコピー代入演算子を削除
	NonCopyable(const NonCopyable&) = delete;
	NonCopyable& operator=(const NonCopyable&) = delete;

};

