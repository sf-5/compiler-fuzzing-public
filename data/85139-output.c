
#if __BITINT_MAXWIDTH__ > 128
struct S1 {
  _BitInt(17) A;
  _BitInt(129) B;
};

int foo(int a) {
  // CHECK: %A1 = getelementptr inbounds %struct.S1, ptr %B, i32 0, i32 0
  // CHECK: store i32 1, ptr %A1
  // CHECK64: %B2 = getelementptr inbounds %struct.S1, ptr %B, i32 0, i32 2
  // WIN32: %B2 = getelementptr inbounds %struct.S1, ptr %B, i32 0, i32 2
  // LIN32: %B2 = getelementptr inbounds %struct.S1, ptr %B, i32 0, i32 1
  // CHECK: %0 = load i32, ptr %a.addr, align 4
  // CHECK: %conv = sext i32 %0 to i129
  // CHECK64: storedv = sext i129 %conv to i192
  // WIN32: storedv = sext i129 %conv to i192
  // LIN32: storedv = sext i129 %conv to i160
  // CHECK64: store i192 %storedv, ptr %B2, align 8
  // WIN32: store i192 %storedv, ptr %B2, align 8
  // LIN32: store i160 %storedv, ptr %B2, align 4
  // CHECK64: %B3 = getelementptr inbounds %struct.S1, ptr %A, i32 0, i32 2
  // WIN32: %B3 = getelementptr inbounds %struct.S1, ptr %A, i32 0, i32 2
  // LIN32: %B3 = getelementptr inbounds %struct.S1, ptr %A, i32 0, i32 1
  // CHECK64: %1 = load i192, ptr %B3, align 8
  // WIN32: %1 = load i192, ptr %B3, align 8
  // LIN32: %1 = load i160, ptr %B3, align 4
  // CHECK64: %loadedv = trunc i192 %1 to i129
  // WIN32: %loadedv = trunc i192 %1 to i129
  // LIN32: %loadedv = trunc i160 %1 to i129
  // CHECK: %conv4 = trunc i129 %loadedv to i32
  struct S1 A = {1, 170};
  struct S1 B = {1, a};
  return (int)A.B + (int)B.B;
}

struct S2 {
  _BitInt(257) A;
  int B;
};

_BitInt(257) bar() {
  // CHECK64: define {{.*}}void @bar(ptr {{.*}} sret([40 x i8]) align 8
  // %[[RET:.+]]) CHECK64: %A = alloca %struct.S2, align 8 CHECK64: %0 =
  // getelementptr inbounds { <{ i8, [39 x i8] }>, i32, [4 x i8] }, ptr %A, i32
  // 0, i32 0 CHECK64: %1 = getelementptr inbounds <{ i8, [39 x i8] }>, ptr %0,
  // i32 0, i32 0 CHECK64: store i8 1, ptr %1, align 8 CHECK64: %2 =
  // getelementptr inbounds { <{ i8, [39 x i8] }>, i32, [4 x i8] }, ptr %A, i32
  // 0, i32 1 CHECK64: store i32 10000, ptr %2, align 8 CHECK64: %A1 =
  // getelementptr inbounds %struct.S2, ptr %A, i32 0, i32 0 CHECK64: %3 = load
  // i320, ptr %A1, align 8 CHECK64: %loadedv = trunc i320 %3 to i257 CHECK64:
  // %storedv = sext i257 %loadedv to i320 CHECK64: store i320 %storedv, ptr
  // %[[RET]], align 8
  struct S2 A = {1, 10000};
  return A.A;
}

void TakesVarargs(int i, ...) {
  // CHECK64: define{{.*}} void @TakesVarargs(i32
  __builtin_va_list args;
  __builtin_va_start(args, i);

  _BitInt(160) A = __builtin_va_arg(args, _BitInt(160));
  // CHECK64: %[[ARG:.+]] = load i192
  // CHECK64: %[[TRUNC:.+]] = trunc i192 %[[ARG]] to i160
  // CHECK64: %[[SEXT:.+]] = sext i160 %[[TRUNC]] to i192
  // CHECK64: store i192 %[[SEXT]], ptr %A, align 8
}

_BitInt(129) * f1(_BitInt(129) * p) {
  // CHECK64: getelementptr inbounds [24 x i8], {{.*}} i64 1
  return p + 1;
}

char *f2(char *p) {
  // CHECK64: getelementptr inbounds i8, {{.*}} i64 24
  return p + sizeof(_BitInt(129));
}

auto BigGlob = (_BitInt(257)) - 1;
// CHECK64: define {{.*}}void @foobar(ptr {{.*}} sret([40 x i8]) align 8
// %[[RET1:.+]])
_BitInt(257) foobar() {
  // CHECK64: %A = alloca [40 x i8], align 8
  // CHECK64: %0 = load i320, ptr @BigGlob, align 8
  // CHECK64: %loadedv = trunc i320 %0 to i257
  // CHECK64: %add = add nsw i257 %loadedv, 1
  // CHECK64: %storedv = sext i257 %add to i320
  // CHECK64: store i320 %storedv, ptr %A, align 8
  // CHECK64: %1 = load i320, ptr %A, align 8
  // CHECK64: %loadedv1 = trunc i320 %1 to i257
  // CHECK64: %storedv2 = sext i257 %loadedv1 to i320
  // CHECK64: store i320 %storedv2, ptr %[[RET1]], align 8
  _BitInt(257) A = BigGlob + 1;
  return A;
}

void f() { static _BitInt(130) p = {10000}; }

struct S3 {
  _BitInt(136) A : 129;
};

void bitField() {
  struct S3 s = {1};
  struct {
    _BitInt(136) A : 48;
    int a;
  } s1 = {s.A};
  s1.A = 36;
  // LIN64: %s = alloca %struct.S3, align 8
  // LIN64: %s1 = alloca %struct.anon, align 8
  // LIN64: call void @llvm.memcpy.p0.p0.i64(ptr align 8 %s, ptr align 8
  // @__const.bitField.s, i64 24, i1 false) LIN64: %bf.load = load i136, ptr %s,
  // align 8 LIN64: %bf.shl = shl i136 %bf.load, 7 LIN64: %bf.ashr = ashr i136
  // %bf.shl, 7 LIN64: %0 = trunc i136 %bf.ashr to i64 LIN64: %bf.load1 = load
  // i64, ptr %s1, align 8 LIN64: %bf.value = and i64 %0, 281474976710655 LIN64:
  // %bf.clear = and i64 %bf.load1, -281474976710656 LIN64: %bf.set = or i64
  // %bf.clear, %bf.value LIN64: store i64 %bf.set, ptr %s1, align 8 LIN64: %a =
  // getelementptr inbounds %struct.anon, ptr %s1, i32 0, i32 1 LIN64: store i32
  // 0, ptr %a, align 8 LIN64: %bf.load2 = load i64, ptr %s1, align 8 LIN64:
  // %bf.clear3 = and i64 %bf.load2, -281474976710656 LIN64: %bf.set4 = or i64
  // %bf.clear3, 36 LIN64: store i64 %bf.set4, ptr %s1, align 8
}

#endif